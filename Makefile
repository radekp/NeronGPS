all:
	@echo make is deprecated. Please teach your fingers to type qbuild.
	@/home/lxuser/Desktop/Qtopia/freerunner/sdk/bin/qbuild -from-make $(MAKE) default
install:
	@echo make is deprecated. Please teach your fingers to type qbuild.
	@/home/lxuser/Desktop/Qtopia/freerunner/sdk/bin/qbuild -from-make $(MAKE) image
cleaninstall:
	@echo make is deprecated. Please teach your fingers to type qbuild.
	@/home/lxuser/Desktop/Qtopia/freerunner/sdk/bin/qbuild -from-make $(MAKE) image
append_install:
	@echo make is deprecated. Please teach your fingers to type qbuild.
	@/home/lxuser/Desktop/Qtopia/freerunner/sdk/bin/qbuild -from-make $(MAKE) append_image
test:
	@echo make is deprecated. Please teach your fingers to type qbuild.
	@/home/lxuser/Desktop/Qtopia/freerunner/sdk/bin/qbuild -from-make $(MAKE) test
lupdate:
	@echo make is deprecated. Please teach your fingers to type qbuild.
	@/home/lxuser/Desktop/Qtopia/freerunner/sdk/bin/qbuild -from-make $(MAKE) lupdate
headers:
	@echo make is deprecated. Please teach your fingers to type qbuild.
	@/home/lxuser/Desktop/Qtopia/freerunner/sdk/bin/qbuild -from-make $(MAKE) headers
regenerate:
	@echo make is deprecated. Please teach your fingers to type qbuild.
	@/home/lxuser/Desktop/Qtopia/freerunner/sdk/bin/qtopiamake
relink:
	@echo make is deprecated. Please teach your fingers to type qbuild.
	@/home/lxuser/Desktop/Qtopia/freerunner/sdk/bin/qbuild -from-make $(MAKE) relink
prep_db:
	@echo make is deprecated. Please teach your fingers to type qbuild.
	@/home/lxuser/Desktop/Qtopia/freerunner/sdk/bin/qbuild -from-make $(MAKE) prep_db
default:
	@echo make is deprecated. Please teach your fingers to type qbuild.
	@/home/lxuser/Desktop/Qtopia/freerunner/sdk/bin/qbuild -from-make $(MAKE) default
clean:
	@echo make is deprecated. Please teach your fingers to type qbuild.
	@/home/lxuser/Desktop/Qtopia/freerunner/sdk/bin/qbuild -from-make $(MAKE) clean
image:
	@echo make is deprecated. Please teach your fingers to type qbuild.
	@/home/lxuser/Desktop/Qtopia/freerunner/sdk/bin/qbuild -from-make $(MAKE) image
packages:
	@echo make is deprecated. Please teach your fingers to type qbuild.
	@/home/lxuser/Desktop/Qtopia/freerunner/sdk/bin/qbuild -from-make $(MAKE) packages
target:
	@echo make is deprecated. Please teach your fingers to type qbuild.
	@/home/lxuser/Desktop/Qtopia/freerunner/sdk/bin/qbuild -from-make $(MAKE) target
print_depends:
	@echo make is deprecated. Please teach your fingers to type qbuild.
	@/home/lxuser/Desktop/Qtopia/freerunner/sdk/bin/qbuild -from-make $(MAKE) print_depends
qmake:
	@echo make is deprecated. Please teach your fingers to type qbuild.
	@/home/lxuser/Desktop/Qtopia/freerunner/sdk/bin/qtopiamake
force_clean:
	@echo make is deprecated. Please teach your fingers to type qbuild.
	@/home/lxuser/Desktop/Qtopia/freerunner/sdk/bin/qbuild -from-make $(MAKE) force_clean
remove_target:
	@echo make is deprecated. Please teach your fingers to type qbuild.
	@/home/lxuser/Desktop/Qtopia/freerunner/sdk/bin/qbuild -from-make $(MAKE) remove_target
.PHONY: all install cleaninstall append_install test lupdate headers regenerate relink prep_db default clean image packages target print_depends qmake force_clean remove_target
