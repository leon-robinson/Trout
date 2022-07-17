.PHONY: all
all: Trout.iso

.PHONY: run
run: Trout.iso
	qemu-system-x86_64 -enable-kvm -cpu host -M q35 -m 2G -cdrom Trout.iso -boot d

limine:
	git clone https://github.com/limine-bootloader/limine.git --branch=v3.0-branch-binary --depth=1
	$(MAKE) -C limine

.PHONY: kernel
kernel:
	$(MAKE) -C kernel

Trout.iso: limine kernel
	rm -rf iso_root
	mkdir -p iso_root
	cp kernel/bin/kernel.elf \
		limine.cfg limine/limine.sys limine/limine-cd.bin limine/limine-cd-efi.bin iso_root/
	xorriso -as mkisofs -b limine-cd.bin \
		-no-emul-boot -boot-load-size 4 -boot-info-table \
		--efi-boot limine-cd-efi.bin \
		-efi-boot-part --efi-boot-image --protective-msdos-label \
		iso_root -o Trout.iso
	limine/limine-deploy Trout.iso
	rm -rf iso_root

.PHONY: clean
clean:
	rm -rf iso_root Trout.iso
	$(MAKE) -C kernel clean