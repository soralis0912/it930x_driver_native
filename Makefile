module=usb-plex
EXTRA_CFLAGS = -DEXPORT_SYMTAB
# passing dynamic URB_BUFSIZE to driver
export URB_BUFSIZE
ALIGNSIZE = 188 

CURRENT = $(shell uname -r)
KDIR = /lib/modules/$(CURRENT)/build
PWD = $(shell pwd)
KDIR26	 := /lib/modules/$(CURRENT)/kernel/drivers/media

Module_Name = tty_Virtual

# Add your debugging flag (or not) to CFLAGS
ifeq ($(DEBUG),y)
  DEBFLAGS = -O -g -DSCULL_DEBUG # "-O" is needed to expand inlines
else
  DEBFLAGS = -O2
endif

ifneq (,$(findstring 2.6.1,$(CURRENT)))
  KDIR_V4L	 := /lib/modules/$(CURRENT)/kernel/drivers/media/dvb/IT930x
else
  KDIR_V4L	 := /lib/modules/$(CURRENT)/kernel/drivers/media/dvb/dvb-usb
endif

LMDIR26	 := /lib/firmware
DEST = /lib/modules/$(CURRENT)/kernel/$(MDIR)

EXTRA_CFLAGS += -Wno-unused-value -Wno-unused-variable -Wno-unused-parameter \
		-Wno-switch $(DEBFLAGS) -I..
	
$(Module_Name)-objs := \
		tty0tty.o
	
obj-m += $(Module_Name).o
	
usb-plex-objs := \
	CXD2856.o R850.o RT710.o TC90522.o MyI2CPortingLayer.o MyFrontend-drv.o \
	it930x-core.o it930x-drv.o it913x-drv.o DIB9090-drv.o MXL69X-drv.o CXD285-drv.o iocontrol.o \
	usb2impl.o user.o IT9300.o brCmd.o brUser.o \
	Si2168B-drv.o IT9133.o tuner.o omega.o cmd.o standard.o \
	Si2147_L1_API.o Si2168B_L1_Properties.o \
	Si2147_L1_Properties.o Si2147_L1_User_Properties.o \
	Si2147_L2_API.o Si2147_L1_Commands.o SiLabs_TER_Tuner_API.o \
	Si2168B_L2_API.o Si_ITE_L0_API.o Si2168B_L1_Commands.o Si2168B_L1_API.o \
	dibantenna.o dibcomponent_monitor.o dibdatabus.o dibdemod.o dib0090.o dib9090_common.o dibtuner.o \
	dibx000_i2c_common.o dibx09x_common.o dibfrontend.o dibfrontend_tune.o dibhost.o dibmac.o dibmonitor.o \
	dibosiris.o dibplatform_linux_kernel.o dibpmu.o dibchannel.o dib0090_cal.o dib0090_agc.o dib0090_reset.o \
	dib0090_efuse.o dib0090_io.o dib7000m_common.o dib9090_fw.o dib7000m_fw.o dib9000_common.o dib7000m_fw_common.o \
	dib7000m_gpio.o dib7000m_power.o \
	mxl_eagle_oem.o  mxlware_eagle_dev_api.o mxlware_eagle_sma_api.o mxl_eagle_oem_debug.o \
	mxlware_eagle_endian.o mxlware_eagle_tuner_api.o mxlware_eagle_atsc_api.o mxlware_eagle_oob_api.o \
	mxlware_eagle_utils.o mxlware_eagle_debug_api.o mxlware_eagle_qam_api.o \
	drvi2c_cxd_ite.o sony_cxd_common.o sony_cxd_demod.o sony_cxd_demod_dvbt.o \
	sony_cxd_demod_dvbt2.o sony_cxd_demod_dvbt2_monitor.o sony_cxd_demod_dvbt_monitor.o sony_cxd_demod_isdbt.o \
	sony_cxd_demod_isdbt_monitor.o sony_cxd_helene.o sony_cxd_i2c.o sony_cxd_integ.o sony_cxd_integ_dvbt.o \
	sony_cxd_integ_dvbt2.o sony_cxd_integ_dvbt_t2.o sony_cxd_integ_isdbt.o sony_cxd_math.o sony_cxd_stopwatch_port.o \
	sony_cxd_tuner_helene.o
	
obj-m += usb-plex.o 
	
default:
	@(cp virtual_com/*.* ./)
	@(cp api/Si2168B_47/*.* ./; cp api/DIB9090/*.* ./; cp api/MXL69X/*.* ./; cp api/CXD285X/*.* ./; cp api/*.* ./; cp src/*.* ./)
	@(cp api/DW_Frontend/R850/*.* ./; cp api/DW_Frontend/RT710/*.* ./; cp api/DW_Frontend/TC90522/*.* ./; cp api/DW_Frontend/CXD2856/*.* ./; cp api/DW_Frontend/*.* ./)
	@(cp api/DW_Frontend/CXD2856/isdb_cable/*.* ./; cp api/DW_Frontend/CXD2856/isdb_terr/*.* ./; cp api/DW_Frontend/CXD2856/isdb_sat/*.* ./; cp api/DW_Frontend/CXD2856/i2c/*.* ./; cp api/DW_Frontend/CXD2856/lib/*.* ./; cp api/DW_Frontend/CXD2856/tuner/*.* ./; cp api/DW_Frontend/CXD2856/tuner/terr_cable_sat_helene/*.* ./; cp api/DW_Frontend/CXD2856/tuner/terr_cable_sat_helene/refcode/*.* ./)
	make -s -C $(KDIR) SUBDIRS=$(PWD) modules
#	insmod $(Module_Name).ko
	
	
install:
	@files='usb-plex.ko tty_Virtual.ko'; for i in $$files;do if [ -e $(KDIR26)/$$i ]; then echo -n "$$i "; rm $(KDIR26)/$$i; fi; done; echo;
	@modprobe -r usb-plex 2>/dev/null; true
	@install -d $(KDIR26) 
	@echo "Installing module:"
	@for i in usb-plex.ko tty_Virtual.ko;do  if [ -e "$$i" ]; then echo -e "$$i "; install -m 644 -c $$i $(KDIR26); fi; done; echo;
	@/sbin/depmod -a ${KERNELRELEASE}	
	@-rm -rf *~ *.o *.ko .*.o.cmd .*.ko.cmd *.mod.c *.*~ *.c *.h modules.order Module.markers Module.symvers .tmp_versions/


remove:
	@echo "Removing old $(KDIR26)/ files:"
	@modprobe -r usb-plex 
	@modprobe -r tty_Virtual
	@files='usb-plex.ko tty_Virtual.ko'; for i in $$files;do if [ -e $(KDIR26)/$$i ]; then echo -e "$$i "; rm $(KDIR26)/$$i; fi; done;
	@/sbin/depmod -a

# ** cu_install , cu_remove
# This two rule is for local using
# cu_install:
#   Let customer could dynamically change URB buffer size.
# cu_remove:
#   Let customer could easily remove local insmod module.

cu_install: 
	@-rm -rf *~ *.o .*.o.cmd .*.ko.cmd *.mod.c *.*~ *.c *.h modules.order Module.markers Module.symvers .tmp_versions \
	@files='usb-plex.ko tty_Virtual.ko'; for i in $$files; do if [ -e $(KDIR_V4L)/$$i ]; then echo -n "$$i "; rm $(KDIR_V4L)/$$i; fi; done; echo;
	@[ -e "tty_Virtual.ko" ] || \
	(printf "\033[91m tty_Virtual.ko do not exist!!!  Please make first \33[0m \n"; exit 1)
	@[ -e "usb-plex.ko" ] || \
	(printf "\033[91m usb-plex.ko do not exist!!!  Please make first \33[0m \n"; exit 1)
	@[ -e "$(KDIR26)/tty_Virtual.ko" ] || \
	(cp tty_Virtual.ko $(KDIR26); depmod; echo "copy tty_Virtual.ko to kernel")
	@[ -e "$(KDIR26)/usb-plex.ko" ] || \
	(cp usb-plex.ko $(KDIR26); depmod; echo "copy usb-plex.ko to kernel")
	@modprobe tty_Virtual;
	@if [ -z "$(URB_BUFSIZE)" ]; then \
		printf "\033[91m Using DEFAULT URB_BUFSIZE \033[0m \n"; \
		modprobe usb-plex; \
	fi
	@if [ -n "$(URB_BUFSIZE)" ]; then \
		if [ $(URB_BUFSIZE) -eq 0 ]; then \
			printf "\033[91m URB_BUFSIZE could not be ZERO!!! Using DEFAULT URB_BUFSIZE \033[0m \n"; \
			modprobe usb-plex; \
		else \
			if [ $$(( $(URB_BUFSIZE) % $(ALIGNSIZE) )) -ne 0 ]; then \
				printf "\033[92m Using ALIGNMENT URB_BUFSIZE=$$(( ($(URB_BUFSIZE) / $(ALIGNSIZE) + 1) * $(ALIGNSIZE) )) \033[0m \n"; \
				modprobe usb-plex URB_BUFSIZE=$(URB_BUFSIZE); \
			else \
				printf "\033[93m Customer set URB_BUFSIZE=$(URB_BUFSIZE) \33[0m \n"; \
				modprobe usb-plex URB_BUFSIZE=$(URB_BUFSIZE); \
			fi \
		fi \
	fi

cu_remove:
	@rmmod  usb-plex
	@rmmod  tty_Virtual
		
clean:
	@-rm -rf *~ *.o *.ko .*.o.cmd .*.ko.cmd *.mod.c *.*~ *.c *.h modules.order Module.markers Module.symvers .tmp_versions/
-include $(KOBJ)/Rules.make
releasesrc:
	@rm -rf release
	@mkdir release; mkdir release/IT913x_SRC; mkdir release/IT913x_SRC/api
	@cp -f api/*.h release/IT913x_SRC/api; cp -af src release/IT913x_SRC; cp -af .*.o.cmd release/IT913x_SRC/api; cp -af *.o release/IT913x_SRC/api
	@cp -f Makefile.release release/IT913x_SRC; mv release/IT913x_SRC/Makefile.release release/IT913x_SRC/Makefile
	@rm release/IT913x_SRC/api/IT913x-*.o; rm release/IT913x_SRC/api/dvb-usb-*.o

-include $(KDIR)/Rules.make

