snes_objects := libco
snes_objects += snes-system
snes_objects += snes-cartridge snes-cheat
snes_objects += snes-memory snes-cpucore snes-smpcore
snes_objects += snes-cpu snes-smp snes-dsp snes-ppu
snes_objects += snes-supergameboy snes-superfx snes-sa1 snes-necdsp
snes_objects += snes-bsx snes-srtc snes-sdd1 snes-spc7110 snes-cx4
snes_objects += snes-obc1 snes-st0018
snes_objects += snes-msu1 snes-serial snes-21fx
objects += $(snes_objects)

ifeq ($(profile),accuracy)
  flags += -DPROFILE_ACCURACY -DDEBUGGER
  snescpu := $(snes)/cpu
  snessmp := $(snes)/smp
  snesdsp := $(snes)/dsp
  snesppu := $(snes)/ppu
else ifeq ($(profile),compatibility)
  flags += -DPROFILE_COMPATIBILITY -DDEBUGGER
  snescpu := $(snes)/cpu
  snessmp := $(snes)/smp
  snesdsp := $(snes)/alt/dsp
  snesppu := $(snes)/alt/ppu
else ifeq ($(profile),performance)
  flags += -DPROFILE_PERFORMANCE -DDEBUGGER
  snescpu := $(snes)/alt/cpu
  snessmp := $(snes)/smp
  snesdsp := $(snes)/alt/dsp
  snesppu := $(snes)/alt/ppu
else ifeq ($(profile),debugger)
  flags += -DPROFILE_COMPATIBILITY -DDEBUGGER
  snescpu := $(snes)/cpu
  snessmp := $(snes)/smp
  snesdsp := $(snes)/alt/dsp
  snesppu := $(snes)/alt/ppu
endif

$(objdir)/libco.o  : libco/libco.c libco/*
$(objdir)/libsnes.o: $(snes)/libsnes/libsnes.cpp $(snes)/libsnes/*

$(objdir)/snes-system.o   : $(snes)/system/system.cpp $(call rwildcard,$(snes)/system/) $(call rwildcard,$(snes)/video/) $(call rwildcard,$(snes)/debugger)
$(objdir)/snes-memory.o   : $(snes)/memory/memory.cpp $(call rwildcard,$(snes)/memory/)
$(objdir)/snes-cpucore.o  : $(snes)/cpu/core/core.cpp $(call rwildcard,$(snes)/cpu/core/)
$(objdir)/snes-smpcore.o  : $(snes)/smp/core/core.cpp $(call rwildcard,$(snes)/smp/core/)
$(objdir)/snes-cpu.o      : $(snescpu)/cpu.cpp $(call rwildcard,$(snescpu)/) $(call rwildcard,$(snes)/debugger) $(call rwildcard,$(snes)/cpu/debugger)
$(objdir)/snes-smp.o      : $(snessmp)/smp.cpp $(call rwildcard,$(snessmp)/) $(call rwildcard,$(snes)/debugger)
$(objdir)/snes-dsp.o      : $(snesdsp)/dsp.cpp $(call rwildcard,$(snesdsp)/) $(call rwildcard,$(snes)/debugger)
$(objdir)/snes-ppu.o      : $(snesppu)/ppu.cpp $(call rwildcard,$(snesppu)/) $(call rwildcard,$(snes)/debugger)
$(objdir)/snes-cartridge.o: $(snes)/cartridge/cartridge.cpp $(call rwilddcard,$(snes)/cartridge/)
$(objdir)/snes-cheat.o    : $(snes)/cheat/cheat.cpp $(call rwildcard,$(snes)/cheat/)

$(objdir)/snes-supergameboy.o: $(snes)/chip/supergameboy/supergameboy.cpp $(call rwildcard,$(snes)/chip/supergameboy/)
$(objdir)/snes-superfx.o     : $(snes)/chip/superfx/superfx.cpp $(call rwildcard,$(snes)/chip/superfx/) $(call rwildcard,$(snes)/debugger)
$(objdir)/snes-sa1.o         : $(snes)/chip/sa1/sa1.cpp $(call rwildcard,$(snes)/chip/sa1/) $(call rwildcard,$(snes)/debugger)
$(objdir)/snes-necdsp.o      : $(snes)/chip/necdsp/necdsp.cpp $(call rwildcard,$(snes)/chip/necdsp/)
$(objdir)/snes-bsx.o         : $(snes)/chip/bsx/bsx.cpp $(snes)/chip/bsx/*
$(objdir)/snes-srtc.o        : $(snes)/chip/srtc/srtc.cpp $(snes)/chip/srtc/*
$(objdir)/snes-sdd1.o        : $(snes)/chip/sdd1/sdd1.cpp $(snes)/chip/sdd1/*
$(objdir)/snes-spc7110.o     : $(snes)/chip/spc7110/spc7110.cpp $(snes)/chip/spc7110/*
$(objdir)/snes-cx4.o         : $(snes)/chip/cx4/cx4.cpp $(snes)/chip/cx4/*
$(objdir)/snes-obc1.o        : $(snes)/chip/obc1/obc1.cpp $(snes)/chip/obc1/*
$(objdir)/snes-st0018.o      : $(snes)/chip/st0018/st0018.cpp $(snes)/chip/st0018/*
$(objdir)/snes-msu1.o        : $(snes)/chip/msu1/msu1.cpp $(snes)/chip/msu1/*
$(objdir)/snes-serial.o      : $(snes)/chip/serial/serial.cpp $(snes)/chip/serial/*
$(objdir)/snes-21fx.o      : $(snes)/chip/21fx/21fx.cpp $(snes)/chip/21fx/*

###########
# library #
###########

snes_objects := $(patsubst %,$(objdir)/%.o,$(snes_objects))

library: $(snes_objects) $(objdir)/libsnes.o
ifeq ($(platform),x)
	ar rcs out/libsnes.a $(snes_objects) $(objdir)/libsnes.o
	$(cpp) -o out/libsnes.so -shared -Wl,-soname,libsnes.so.1 $(snes_objects) $(objdir)/libsnes.o
else ifeq ($(platform),osx)
	ar rcs out/libsnes.a $(snes_objects) $(objdir)/libsnes.o
	$(cpp) -o out/libsnes.dylib -install_name @executable_path/../Libraries/libsnes.dylib -shared -dynamiclib $(snes_objects) $(objdir)/libsnes.o
else ifeq ($(platform),$(filter $(platform),win msys))
	$(cpp) -o out/snes.dll -shared -Wl,--out-implib,libsnes.a $(snes_objects) $(objdir)/libsnes.o
endif

library-install:
ifeq ($(platform),x)
	install -D -m 755 out/libsnes.a $(DESTDIR)$(prefix)/lib/libsnes.a
	install -D -m 755 out/libsnes.so $(DESTDIR)$(prefix)/lib/libsnes.so
	ldconfig -n $(DESTDIR)$(prefix)/lib
else ifeq ($(platform),osx)
	cp out/libsnes.dylib /usr/local/lib/libsnes.dylib
endif

library-uninstall:
ifeq ($(platform),x)
	rm $(DESTDIR)$(prefix)/lib/libsnes.a
	rm $(DESTDIR)$(prefix)/lib/libsnes.so
else ifeq ($(platform),osx)
	rm /usr/local/lib/libsnes.dylib
endif
