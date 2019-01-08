#ifndef INFO_H
#define INFO_H


class info
{
    public:
        info();
        virtual ~info();
        info& operator=(const info& other);

        unsigned char Getziel[3]() { return ziel[3]; }
        void Setziel[3](unsigned char val) { ziel[3] = val; }
        unsigned char Getquelle[3]() { return quelle[3]; }
        void Setquelle[3](unsigned char val) { quelle[3] = val; }
        unsigned char Getfunction() { return function; }
        void Setfunction(unsigned char val) { function = val; }
        unsigned char Getaddress() { return address; }
        void Setaddress(unsigned char val) { address = val; }
        unsigned char Getjob() { return job; }
        void Setjob(unsigned char val) { job = val; }
        unsigned char Getchannel() { return channel; }
        void Setchannel(unsigned char val) { channel = val; }

    protected:

    private:
        unsigned char ziel[3];
        unsigned char quelle[3];
        unsigned char function;
        unsigned char address;
        unsigned char job;
        unsigned char channel;
};

#endif // INFO_H
