#include <snes.hpp>

#define S21FX_CPP
namespace SNES {

S21FX s21fx;

uint8 S21FX::mmio_read(unsigned addr) {
  addr &= 0x40ffff;

  if(addr >= 0x2184 && addr <= 0x21fd) return ram[addr - 0x2184];

  if(addr == 0x21fe) {
    if(opened()) {
      uint8 flags = 1 << 5;
      if (fx_readable) flags |= fx_readable() << 7;
      if (fx_writable) flags |= fx_writable() << 6;
      return flags;
    } else {
      return 0;
    }
  }

  if(addr == 0x21ff) {
    if(Memory::debugger_access()) return 0x00;
    if(opened() && fx_read) return fx_read();
  }

  return 0x00;
}

void S21FX::mmio_write(unsigned addr, uint8 data) {
  addr &= 0x40ffff;

  if(addr == 0x21ff && opened() && fx_write) {
    fx_write(data);
  }
}

void S21FX::enable() {
  unload();

  for(auto& byte : ram) byte = 0xdb;  //stp
  ram[0] = 0x6c;  //jmp ($fffc)
  ram[1] = 0xfc;
  ram[2] = 0xff;

  file fp;
  if (fp.open("21fx.rom", file::mode::read)) {
    fp.read(ram, sizeof(ram));
  }
  fp.close();

  string name = notdir(cartridge.basename());
  string path = dir(cartridge.basename());
  if(open(name, path)) {
    fx_init = sym("fx_init");
    fx_quit = sym("fx_quit");
    fx_read = sym("fx_read");
    fx_write = sym("fx_write");
    fx_readable = sym("fx_readable");
    fx_writable = sym("fx_writable");

    if(fx_init) fx_init();
  }
}

void S21FX::unload() {
  if(opened()) {
    if(fx_quit) fx_quit();
    close();
  }
}

}
