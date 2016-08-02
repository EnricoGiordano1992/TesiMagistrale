/// class Top - 
class Top {
  // Attributes
public:
  sc_fifo<int> in1_fifo;
  sc_fifo<int> in2_fifo;
  sc_fifo<int> out1_fifo;
  sc_fifo<int> out2_fifo;
  rc_portal<sc_fifo_in<int>> in1_portal;
  rc_portal<sc_fifo_in<int>> in2_portal;
  rc_portal<sc_fifo_out<int>> out1_portal;
  rc_portal<sc_fifo_out<int>> out2_portal;
  rc_control ctrl;
  Maximum_rc m1;
  Serialize_rc m2;
  Swap_rc m3;
  Repeat_rc m4;
  Consume_rc m5;
  staticPortMap stat_portmap;
  rc_switch_connector<dynPortMap> connector;
private:
  rc_reconfigurable* active_module;
  // Operations
public:
  SC_CTOR ( Top);
private:
  void activate (rc_reconfigurable& module);
  void control_proc ();
  void producer_proc ();
  void monitor_proc ();
};

