class S21FX : public MMIO, public library, public property<Socket> {
public:
  void enable();
  void unload();

  uint8 mmio_read(unsigned addr);
  void mmio_write(unsigned addr, uint8 data);

private:
  function<void ()> fx_init;
  function<void ()> fx_quit;
  function<uint8 ()> fx_read;
  function<void (uint8)> fx_write;
  function<bool ()> fx_readable;
  function<bool ()> fx_writable;

  uint8 ram[122];
};

extern S21FX s21fx;
