/*
 * ComReceiver.cpp
 *
 * Created: 15.04.2017 06:14:06
 *  Author: Christof
 */
#include "ComReceiver.h"
#include "CommandFunctions.h"
#include <ctype.h>
#include "ledHardware.h"

uint8_t rec_state_KNET = RCST_WAIT;
uint8_t function_KNET=0;
uint8_t job_KNET=0;
uint8_t address_KNET=0;
uint8_t crc_KNET=CRC_NO;
char *parameter_text_KNET=NULL;
uint8_t parameter_text_length_KNET;
uint8_t parameter_text_pointer_KNET;

char *temp_parameter_text_KNET=NULL;
uint8_t temp_parameter_text_length_KNET;
uint8_t temp_parameter_text_pointer_KNET;

uint8_t bootloader_attention;		// nur wenn true, dann darf Bootloader gestartet werden.
uint8_t reset_attention;			// nur wenn true, dann darf Reset ausgeloest werden.

void (*bootloader)( void ) = (void (*)(void)) (BOOT_SECTION_START/2);       // Set up function pointer
void (*reset)( void ) = (void (*)(void)) 0x0000;       // Set up function pointer

#define NUM_COMMANDS 13
#define NUM_INFORMATION 8
COMMAND commands[NUM_COMMANDS] =
{
  {'-','-',CUSTOMER,NOPARAMETER,0,jobGotCRCError}, // Achtung, muss immer der erste sein
  {'S','K',CUSTOMER,STRING,16,jobSetSecurityKey},
  {'S','k',CUSTOMER,NOPARAMETER,0,jobGetSecurityKey},
  {'C','t',CUSTOMER,NOPARAMETER,0,jobGetCTemperatureSensor},
  {'C','h',CUSTOMER,NOPARAMETER,0,jobGetCHumiditySensor},
  {'C','d',CUSTOMER,NOPARAMETER,0,jobGetCDewPointSensor},
  {'C','a',CUSTOMER,NOPARAMETER,0,jobGetCAbsHumiditySensor},
  {'S','C',DEVELOPMENT,NOPARAMETER,0,jobGetCompilationDate},
  {'S','T',DEVELOPMENT,NOPARAMETER,0,jobGetCompilationTime},
  {'S','m',PRODUCTION,NOPARAMETER,0,jobGetFreeMemory},
  {'M','r',PRODUCTION,NOPARAMETER,16,jobGetMeasureRate},
  {'M','R',PRODUCTION,UINT_16,16,jobSetMeasureRate},
  {'C','d',CUSTOMER,FLOAT,1,jobGotExternalTemperature},
};

INFORMATION information[NUM_INFORMATION]=
{
  {"CQ",'C','1','t',FLOAT,1,(void*)&fExternalTemperature,NULL},
  {"CQ",'C','1','h',FLOAT,1,(void*)&fExternalHumidity,NULL},
  {"C1",'C','1','p',FLOAT,1,(void*)&fExternalPressure,NULL},
  {"CQ",'C','1','d',FLOAT,1,(void*)&fExternalDewPoint,NULL},
  {"DT",'e','c','n',UINT_8,1,(void*)&gotEmailNumber,NULL},
  {"DT",'t','1','s',FLOAT,1,(void*)&MqttTime,gotNewMqttTime},
  {"H1",'H','1','a',STRING,5,(void*)&heaterAlarm,gotHeaterAlarmInfo},
  {"H1",'H','1','w',STRING,5,(void*)&heaterWater,gotHeaterAlarmInfo}
};

void doJob(Communication *output)
{
	if (  (rec_state_KNET == RCST_DO_JOB) && (job_KNET > 0) )
	{
    if(isBroadcast==false)
    {
      if (SecurityLevel < commands[job_KNET-1].security)
      {
         output->sendAnswer(fehler_text[SECURITY_ERROR],quelle_KNET,commands[job_KNET-1].function,address_KNET,commands[job_KNET-1].job,false);
      }
      else
      {
        if ( job_KNET<=NUM_COMMANDS )
        {
          commands[job_KNET-1].commandFunction(output,commands[job_KNET-1].function,address_KNET,commands[job_KNET-1].job, parameter_text_KNET);
        }
      }
		}
		else // isBroadcast==true
		{
      switch( information[job_KNET-1].ptype )
      {
        case FLOAT:
          *( (float *)information[job_KNET-1].targetVariable ) = ((float *)parameter_text_KNET)[0];
        break;
        case UINT_32:
          *( (uint32_t *)information[job_KNET-1].targetVariable ) = ((uint32_t *)parameter_text_KNET)[0];
        break;
        case UINT_8:
          *( (uint8_t *)information[job_KNET-1].targetVariable ) = ((uint8_t *)parameter_text_KNET)[0];
        break;
        case STRING: // nicht getestet
          strncpy( (char *)information[job_KNET-1].targetVariable , (char *)parameter_text_KNET, (char *)information[job_KNET-1].pLength);
        break;
      }
      if(information[job_KNET-1].gotNewInformation != NULL)
        information[job_KNET-1].gotNewInformation();
      _delay_ms(30);
		}
		free_parameter_KNET();
		rec_state_KNET = RCST_WAIT;
		function_KNET = 0;
		job_KNET = 0;
	}
}

/* "2019-04-18-06-36-02" */
// https://stackoverflow.com/questions/5754315/c-convert-char-to-timestamp/5754417#5754417
// https://stackoverflow.com/questions/1859201/add-seconds-to-a-date
void gotNewMqttTime()
{
  cli();
  secondsCounter = uint32_t(MqttTime);
  sei();
}

void gotHeaterAlarmInfo()
{
  if((strcmp(heaterAlarm,"ON")==0) | (strcmp(heaterWater,"ON")==0))
    heaterCollectionAlarm = true;
  else
    heaterCollectionAlarm = false;
}

void comStateMachine(Communication *input)
{
	uint8_t ready,i;
	uint8_t error = NO_ERROR;
	char act_char,temp;
	static char crcString[5];
	static uint8_t crcIndex;
	uint8_t length;

	char infoType;
	if( input->getChar(act_char) == true )
	{
		if( false )
		{
			rec_state_KNET = RCST_L1;
		}
		else
		{
      PORTA.OUT = ~rec_state_KNET;
			switch( rec_state_KNET )
			{
				case RCST_WAIT:
					if( act_char=='#' )
					{
						crcIndex = 0;
						crcGlobal.Reset();
						isBroadcast = false;
						crcGlobal.Data(act_char);
						rec_state_KNET = RCST_L1;
 					}
				break;
				case RCST_L1:
					if( isxdigit(act_char)!=false )
					{
						if( act_char<58)
							length = 16*(act_char-48);
						else
						{
							act_char = tolower(act_char);
							length = 16*(act_char-87);
						}
						crcGlobal.Data(act_char);
						rec_state_KNET = RCST_L2;
					}
					else
						rec_state_KNET = RCST_WAIT;
				break;
				case RCST_L2:
					if( isxdigit(act_char)!=false )
					{
						if( act_char<58)
							length += (act_char-48);
						else
						{
							act_char = tolower(act_char);
							length += (act_char-87);
						}
						crcGlobal.Data(act_char);
						rec_state_KNET = RCST_HEADER;
					}
					else
						rec_state_KNET = RCST_WAIT;
				break;
				case RCST_HEADER:
					if ( (act_char&4)==4 )
					{
						crc_KNET=CRC_YES;
						crcGlobal.Data(act_char);
					}
					else
					{
						crc_KNET=CRC_NO;
					}
					rec_state_KNET = RCST_Z1;
				break;
				case RCST_Z1:
					if(crc_KNET==CRC_YES)
						crcGlobal.Data(act_char);
					if( act_char==Node[0] )
                        rec_state_KNET = RCST_Z2;
                    else
                    {
                        if( act_char=='B' )
                            rec_state_KNET = RCST_BR2;
                        else
                            rec_state_KNET= RCST_WAIT;
                    }

/*					switch ( act_char )
					{
						case Node[0]:
							rec_state_KNET = RCST_Z2;
						break;
						case 'B':
							rec_state_KNET = RCST_BR2;
						break;
						default:
							rec_state_KNET= RCST_WAIT;
					}*/
				break;
				case RCST_Z2:
					if( act_char==Node[1] )
                    {
						if(crc_KNET==CRC_YES)
							crcGlobal.Data(act_char);
						rec_state_KNET = RCST_Q1;
//						LED_ROT_ON;
                    }
                    else
                    {
                        rec_state_KNET= RCST_WAIT;
                    }
/*

					if ( act_char==Node[1] )
					{
						if(crc_KNET==CRC_YES)
							crcGlobal.Data(act_char);
						rec_state_KNET = RCST_Q1;
						LED_ROT_ON;
					}
					else
					{
						rec_state_KNET= RCST_WAIT;
					}*/
				break;
				case RCST_BR2:
          if ( act_char=='R' )
          {
            if(crc_KNET==CRC_YES)
              crcGlobal.Data(act_char);
            rec_state_KNET = RCST_Q1;
            isBroadcast = true;
         }
          else
          {
            rec_state_KNET= RCST_WAIT;
          }
				break;
				case RCST_Q1:
					if(crc_KNET==CRC_YES)
						crcGlobal.Data(act_char);
					quelle_KNET[0]=act_char;
					rec_state_KNET = RCST_Q2;
				break;
				case RCST_Q2:
					if(crc_KNET==CRC_YES)
						crcGlobal.Data(act_char);
					quelle_KNET[1]=act_char;
					quelle_KNET[2]=0;
					rec_state_KNET = RCST_KEADER;
				break;
				case RCST_KEADER:
					infoType=act_char;
					if (infoType=='S')
					{
						if(crc_KNET==CRC_YES)
							crcGlobal.Data(act_char);
						rec_state_KNET=RCST_WAIT_FUNCTION;
					}
					else
					{
						rec_state_KNET=RCST_WAIT;
					}
				break;
				case RCST_WAIT_FUNCTION:
					rec_state_KNET = RCST_WAIT_ADDRESS;
					ready = false;
					temp = 0;
					i = 0;
					if(isBroadcast==false)
					{
            do
            {
              if(commands[i].function == act_char)
              {
                temp = act_char;
                ready = true;
                if(crc_KNET==CRC_YES)
                  crcGlobal.Data(act_char);
              }
              i++;
              if(i==NUM_COMMANDS)
                ready = true;
            }while (!ready);
					}
					else
					{
           do
            {
              if(  (information[i].function==act_char) & (information[i].quelle[0]==quelle_KNET[0]) & (information[i].quelle[1]==quelle_KNET[1])  )
              {
                temp = act_char;
                ready = true;
                if(crc_KNET==CRC_YES)
                  crcGlobal.Data(act_char);
              }
              i++;
              if(i==NUM_INFORMATION)
                ready = true;
            }while (!ready);
					}
					function_KNET = temp;
				break;
				case RCST_WAIT_ADDRESS:

					rec_state_KNET = RCST_WAIT_JOB;
					address_KNET = act_char;
                    if(crc_KNET==CRC_YES)
                        crcGlobal.Data(act_char);
				break;
				case RCST_WAIT_JOB:
					rec_state_KNET = RCST_NO_PARAMETER;
					ready = false;
					temp = 0;
					i = 0;
					if(isBroadcast==false)
					{
            do
            {
              if(commands[i].function == function_KNET)
              {
                if(commands[i].job == act_char)
                {
                  temp = i+1;  // Achtung: job_KNET ist immer eins größer als der Index
                  ready = true;
                  if(crc_KNET==CRC_YES)
                    crcGlobal.Data(act_char);
                }
              }
              i++;
              if(i==NUM_COMMANDS)
                ready = true;
            }while (!ready);
					}
					else
					{
            do
            {
              if(  (information[i].function == function_KNET) && (information[i].address == address_KNET) && (information[i].quelle[0]==quelle_KNET[0]) && (information[i].quelle[1]==quelle_KNET[1]) )
              {
                if(information[i].job == act_char)
                {
                  temp = i+1;  // Achtung: job_KNET ist immer eins größer als der Index
                  ready = true;
                  if(crc_KNET==CRC_YES)
                    crcGlobal.Data(act_char);
                }
              }
              i++;
              if(i==NUM_INFORMATION)
                ready = true;
            }while (!ready);
					}
					job_KNET = temp;
					if (job_KNET==0)
					{
						bootloader_attention = false;
						function_KNET = 0;
						job_KNET = 0;
						rec_state_KNET = RCST_WAIT;
					}
					else
					{
            uint8_t ptype,pLength;
            if( isBroadcast==false)
            {
              ptype = commands[job_KNET-1].ptype;
              pLength = commands[job_KNET-1].pLength;
            }
            else
            {
              ptype = information[job_KNET-1].ptype;
              pLength = information[job_KNET-1].pLength;
            }
						if( ptype != NOPARAMETER )
						{
							parameter_text_KNET = (char*) getMemory(ptype,pLength);
							if (parameter_text_KNET==NULL)
							{
								input->sendInfo("!!!!!Error!!!!!!","BR");
							}
							parameter_text_length_KNET = pLength;
							if( ptype != STRING )
								temp_parameter_text_KNET = (char *) getMemory(STRING,MAX_TEMP_STRING);
							rec_state_KNET = RCST_GET_DATATYPE;
						}
						else
							rec_state_KNET = RCST_NO_PARAMETER;
						parameter_text_pointer_KNET = 0;
						temp_parameter_text_pointer_KNET = 0;
					}
				break;
				case RCST_NO_PARAMETER: // dann muss der Datentyp = '?' sein
					if( act_char=='?' )
					{
						if(crc_KNET==CRC_YES)
						{
							crcGlobal.Data(act_char);
							rec_state_KNET = RCST_CRC;
						}
						else
							rec_state_KNET = RCST_WAIT_END1;
					}
					else
						rec_state_KNET = RCST_WAIT;
				break;
				case RCST_GET_DATATYPE: // einziger bekannter Datentyp : 'T'
					if( (act_char=='F') | (act_char=='t')  | (act_char=='T') )
					{
						if(crc_KNET==CRC_YES)
							crcGlobal.Data(act_char);
						rec_state_KNET = RCST_GET_PARAMETER;
					}
					else
						rec_state_KNET = RCST_WAIT;
				break;

				case RCST_CRC:
					if ( isxdigit(act_char) )
					{
						crcString[crcIndex] =  act_char;
						crcIndex++;
						if (crcIndex>=4)
						{
							crc_KNET=CRC_IO;
							if(crcGlobal.compare(crcString) != true )
							{
								job_KNET = 1;	// das ist der CRC-Error-Job
							}
							rec_state_KNET = RCST_WAIT_END1;
						}
					}
					else
					{
						rec_state_KNET = RCST_WAIT;
						job_KNET = 0;
						free_parameter_KNET();
					}
				break;

				case RCST_WAIT_END1:
					if( (act_char=='\r') | (act_char=='\n') )
						rec_state_KNET = RCST_WAIT_END2;
					else
					{
						rec_state_KNET = RCST_WAIT;
						job_KNET = 0;
						free_parameter_KNET();
					}
				break;
				case RCST_WAIT_END2:
					//if( act_char=='\n' )
					if( (act_char=='\r') | (act_char=='\n') )
						rec_state_KNET = RCST_DO_JOB;
					else
					{
						rec_state_KNET = RCST_WAIT;
						job_KNET = 0;
						free_parameter_KNET();
					}
				break;
				case RCST_GET_PARAMETER:
					if(crc_KNET==CRC_YES)
						crcGlobal.Data(act_char);
          uint8_t ptype,pLength;
          if( isBroadcast==false)
          {
            ptype = commands[job_KNET-1].ptype;
            pLength = commands[job_KNET-1].pLength;
          }
          else
          {
            ptype = information[job_KNET-1].ptype;
            pLength = information[job_KNET-1].pLength;
          }
					if ( ptype==STRING )
					{
						if( (act_char=='<') )					// Parameterende
						{
							if(crc_KNET==CRC_YES)
								rec_state_KNET = RCST_CRC;
							else
								rec_state_KNET = RCST_WAIT_END1;

							parameter_text_KNET[parameter_text_pointer_KNET] = 0;
//							input->println("-----------------");
//							input->println(parameter_text_KNET);
						}
						else
						{
							if( parameter_text_pointer_KNET < parameter_text_length_KNET-2 )
							{
								parameter_text_KNET[parameter_text_pointer_KNET] = act_char;
								parameter_text_pointer_KNET++;
							}
							else // zu langer Parameter
							{
								rec_state_KNET = RCST_WAIT;
								error = ERROR_PARAMETER;
								function_KNET = 0;
								free_parameter_KNET();
							}

						}
					} // if STRING
					else // if some Number-Parameter
					{
						if ((act_char=='<') || (act_char==','))
						{
							errno = 0;
							temp_parameter_text_KNET[temp_parameter_text_pointer_KNET] = 0;		// Zahlenstring abschießen
							if ( parameter_text_pointer_KNET < pLength )   // wird noch ein Parameter erwartet?
							{
								uint32_t wert;
								switch(ptype)
								{
									case UINT_8:
										uint8_t *pointer_u8;
										pointer_u8 =  (uint8_t*) parameter_text_KNET;
										wert = strtoul(temp_parameter_text_KNET,NULL,0);
										if(wert<256)
											pointer_u8[parameter_text_pointer_KNET] = (uint8_t) wert;
										else
											error = ERROR_PARAMETER;
									break;
									case UINT_16:
										uint16_t *pointer_u16;
										pointer_u16 =  (uint16_t*) parameter_text_KNET;
										wert = strtoul(temp_parameter_text_KNET,NULL,0);
										if(wert<65536)
											pointer_u16[parameter_text_pointer_KNET] = (uint16_t) wert;
										else
											error = ERROR_PARAMETER;
//										input->println(temp_parameter_text_KNET);	!!!!!!!!!!!!!!!!!auskommentiert!!!!!!!!!!!!!!!!!!!!
//										input->pformat("Wert: %\>d\n",wert);		!!!!!!!!!!!!!!!!!auskommentiert!!!!!!!!!!!!!!!!!!!!
									break;
									case UINT_32:
										uint32_t *pointer_u32;
										pointer_u32 =  (uint32_t*) parameter_text_KNET;
										pointer_u32[parameter_text_pointer_KNET] = strtoul(temp_parameter_text_KNET,NULL,0);
									break;
									case FLOAT:
										double *pointer_d;
										pointer_d =  (double*) parameter_text_KNET;
										pointer_d[parameter_text_pointer_KNET] = strtod(temp_parameter_text_KNET,NULL);
									break;
								}
							}
							else
								error = ERROR_PARAMETER;
							if( parameter_text_pointer_KNET < parameter_text_length_KNET-1 ) // Zeiger auf nächsten Parameter
							{
								parameter_text_pointer_KNET++;
								temp_parameter_text_pointer_KNET = 0;						// zurücksetzen für nächsten Parameter
							}
							else
                            {
                                    ;
                            } // hier noch abfangen falls zu viele Parameter eingeben wurden
							if ( errno != 0)
								error = ERROR_PARAMETER;
							if ((act_char=='<'))
							{
								if(crc_KNET==CRC_YES)
									rec_state_KNET = RCST_CRC;
								else
									rec_state_KNET = RCST_WAIT_END1;
							}
							/* hier noch abfangen, falls zu wenige Parameter eingegeben wurden ************************ */
						}
						else // weiterer Character eines Parameters
						{
							if( temp_parameter_text_pointer_KNET < MAX_TEMP_STRING-2 )
							{
								temp_parameter_text_KNET[temp_parameter_text_pointer_KNET] = act_char;
								temp_parameter_text_pointer_KNET++;
							}
							else // zu langer Parameter
								error = ERROR_JOB;
						}
						if ( error != NO_ERROR )
						{
								function_KNET = 0;
								rec_state_KNET = RCST_WAIT;
								free_parameter_KNET();
						}
					}
				break; // case RCST_GET_PARAMETER

			} // end of switch
//			input->pformat("State: %x, char:%x, job:%d\r\n",rec_state_KNET,act_char,job_KNET);
		}
	}
}

void *getMemory(uint8_t type,uint8_t num)
{
uint8_t size=1;
void *mem=NULL;
	switch(type)
	{
		case STRING:
			size = 1;
		break;
		case UINT_8:
			size = 1;
		break;
		case UINT_16:
			size = 2;
		break;
		case UINT_32:
			size = 4;
		break;
		case FLOAT:
			size = sizeof(double);
		break;
		default:
			size = -1;

	}
	if (size>0)
	{
		mem =  malloc(size*num);

	}
	return( mem );
}

void free_parameter_KNET(void)
{
	if (parameter_text_KNET)
	{
		free( parameter_text_KNET );
		parameter_text_KNET = NULL;
		parameter_text_length_KNET = 0;
	}
	if (temp_parameter_text_KNET != NULL)
	{
		free( temp_parameter_text_KNET );
		temp_parameter_text_KNET = NULL;
		temp_parameter_text_pointer_KNET = 0;
	}
}

