#include "mem/port.hh"
#include "params/SimpleMemobj.hh"
#include "sim/sim_object.hh"

class SimpleMemobj : public MemObject
{
  private:

        class CPUSidePort : public SlavePort
    {
      private:
        SimpleMemobj *owner;

      public:
        CPUSidePort(const std::string& name, SimpleMemobj *owner) :
            SlavePort(name, owner), owner(owner)
        { }

        AddrRangeList getAddrRanges() const override;

      protected:
        Tick recvAtomic(PacketPtr pkt) override { panic("recvAtomic unimpl."); }
        void recvFunctional(PacketPtr pkt) override;
        bool recvTimingReq(PacketPtr pkt) override;
        void recvRespRetry() override;
    };

        class MemSidePort : public MasterPort
    {
      private:
        SimpleMemobj *owner;

      public:
        MemSidePort(const std::string& name, SimpleMemobj *owner) :
            MasterPort(name, owner), owner(owner)
        { }

      protected:
        bool recvTimingResp(PacketPtr pkt) override;
        void recvReqRetry() override;
        void recvRangeChange() override;
    };

    CPUSidePort instPort;
    CPUSidePort dataPort;
    MemSidePort memPort;


  public:

    /** constructor
     */
    SimpleMemobj(SimpleMemobjParams *params);
    void startup() override; //!!
    
    BaseMasterPort& getMasterPort(const std::string& if_name, PortID idx=InvalidPortID) override;
    BaseSlavePort& getSlavePort(const std::string& if_name, PortID idx=InvalidPortID) override;
};