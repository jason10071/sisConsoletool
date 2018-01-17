all: BUILD_LIB
	@cd CT               && $(MAKE)
	
BUILD_LIB:
	@cd SiSLog           && $(MAKE)
	@cd SiSExpressionLib && $(MAKE)
	@cd SiSDeviceIO      && $(MAKE)
	@cd SiSProcedure     && $(MAKE)
	@cd CTBase           && $(MAKE)
	
CLEAN_LIB:
	@cd SiSLog           && $(MAKE) clean
	@cd SiSExpressionLib && $(MAKE) clean
	@cd SiSDeviceIO      && $(MAKE) clean
	@cd SiSProcedure     && $(MAKE) clean
	@cd CTBase           && $(MAKE) clean

clean:CLEAN_LIB
	@cd CT               && $(MAKE) clean
	
CLEAN_LIB_ALL:
	@cd SiSLog           && $(MAKE) clean_all
	@cd SiSExpressionLib && $(MAKE) clean_all
	@cd SiSDeviceIO      && $(MAKE) clean_all
	@cd SiSProcedure     && $(MAKE) clean_all
	@cd CTBase           && $(MAKE) clean_all
	
clean_all:CLEAN_LIB_ALL
	@cd CT               && $(MAKE) clean_all
	@rm -vfr bin lib
