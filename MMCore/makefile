
RM := del
PROJ_NAME := MMCore
PREFIX := 
POSTFIX := 
EXTENSION := .lib
TARGET_NAME := $(PREFIX)$(PROJ_NAME)$(POSTFIX)$(EXTENSION)
ifeq ($(DEBUG), 1)
	CONFIG := Debug
	PF := d
else
	CONFIG := Release
	PF := r
endif
TARGET_PATH := $(CONFIG)
TARGET := $(TARGET_PATH)\$(TARGET_NAME)

OBJ_FILES := $(patsubst %.cpp,%.o,$(wildcard *.cpp))
MM_OBJ_FILES:=DeviceUtils.o ImgBuffer.o



MM_ROOT := ..\..
THIRD_PARTY := $(MM_ROOT)\3rdpartypublic
IDIR := $(THIRD_PARTY)\boost $(THIRD_PARTY)\zlib
IFLAGS := $(patsubst %,/I%,$(IDIR)) /I"$(WIN_SDK)\Include"
GCC_IFLAGS := $(patsubst %,-I%,$(THIRD_PARTY)/boost $(THIRD_PARTY)/boost/boost $(THIRD_PARTY)/zlib)
PREPROCS := _WINDOWS _USRDLL MMCOREJ_WRAP_EXPORTS _WINDLL _MBCS WIN32 _LIB MODULE_EXPORTS
ifeq ($(DEBUG), 1)
	PREPROCS += _DEBUG
	CFLAGS :=/c /ZI /nologo /W4 /WX- /Od /Oy- /Gm /EHa /RTC1 /MDd /GS /fp:precise /Zc:wchar_t /Zc:forScope /GR /Gd /wd"4290" /analyze- /errorReport:queue
else
	PREPROCS += NDEBUG
	CFLAGS :=/c /Zi /nologo /W4 /WX- /O2 /Oy- /Gm- /EHa /MD /GS /fp:precise /Zc:wchar_t /Zc:forScope /GR /Gd /wd"4290" /analyze- /errorReport:queue
endif
PREPROC_FLAGS := $(patsubst %,/D%,$(PREPROCS))
CXX := cl
LINK_FLAGS := /NOLOGO
LIBPATH := $(THIRD_PARTY)\boost\stage\lib $(THIRD_PARTY)\zlib\bin_Win32
LIB_FLAGS := $(patsubst %,/LIBPATH:%,$(LIBPATH)) /LIBPATH:"$(WIN_SDK)\Lib" 
LIBS := zlib.lib

.PHONY: all clean pre-build main-build post-build
all: pre-build main-build post-build

-include $(OBJ_FILES:.o=.d)
-include $(MM_OBJ_FILES:.o=.d)

#@vcvars32.bat
pre-build:
	@echo.
	@echo MMCore: pre-building...
	$(if $(wildcard $(TARGET_PATH)), , -@mkdir $(subst /,\,$(TARGET_PATH)))
	$(if $(wildcard $(TARGET_PATH)/*.o),-move /Y "$(TARGET_PATH)\*.o" ./,)
	$(if $(wildcard $(TARGET_PATH)/*.d),-copy /Y "$(TARGET_PATH)\*.d" .,)
	$(if $(wildcard $(TARGET_PATH)/*.idb),-move /Y "$(TARGET_PATH)\*.idb" ./,)
	$(if $(wildcard $(TARGET_PATH)/*.pdb),-move /Y "$(TARGET_PATH)\*.pdb" ./,)
	@echo.
	
main-build: $(TARGET)

$(TARGET): $(OBJ_FILES) $(MM_OBJ_FILES)
	@echo Building target: $@, configuration: $(CONFIG).
	@echo Invoking: Linker (link)
	lib $(LINK_FLAGS) /OUT:$(TARGET) $^ $(LIBS) $(LIB_FLAGS)
	@echo Finished building target: $@
	@echo.
	
$(OBJ_FILES): %.o: %.cpp
	@echo Building file: $<
	@echo Invoking: C++ Compiler (cl)
	$(CXX) $(CFLAGS) $(PREPROC_FLAGS) $(IFLAGS) /Fo$@ $<
	gcc -MM $(GCC_IFLAGS) $< > $(patsubst %.o,%.d,$@)
	@echo Finished building: $<, out: $@
	@echo.
	
$(MM_OBJ_FILES): %.o: ../MMDevice/%.cpp
	@echo Building file: $<
	@echo Invoking: C++ Compiler (cl)
	$(CXX) $(CFLAGS) $(PREPROC_FLAGS) $(IFLAGS) /Fo$@ $<
	gcc -MM $(GCC_IFLAGS) $< > $(patsubst %.o,%.d,$@)
	@echo Finished building: $<, out: $@
	@echo.
	
post-build:
	@echo MMCore: post-build:
	-move /Y *.o "$(TARGET_PATH)\\"
	-copy /Y *.d "$(TARGET_PATH)"
	$(if $(wildcard $(TARGET_PATH)\*.idb),-move /Y *.idb "$(TARGET_PATH)\\",)
	$(if $(wildcard $(TARGET_PATH)\*.pdb),-move /Y *.pdb "$(TARGET_PATH)\\",)
	copy $(TARGET) $(MM_ROOT)\micromanager\lib_Win32
	copy $(TARGET) $(MM_ROOT)\micromanager\lib_Win32\$(PREFIX)$(PROJ_NAME)$(POSTFIX)$(PF)$(EXTENSION)
	@echo.
	
clean:
	@echo Cleaning project...
	$(if $(wildcard $(TARGET_PATH)\*.lib), -@$(RM) $(TARGET_PATH)\*.lib,)
	$(if $(wildcard $(TARGET_PATH)\*.o), -@$(RM) $(TARGET_PATH)\*.o,)
	$(if $(wildcard $(TARGET_PATH)\*.d), -@$(RM) $(TARGET_PATH)\*.d,)
	$(if $(wildcard *.d), -@$(RM) *.d,)
	$(if $(wildcard $(TARGET_PATH)\*.idb), -@$(RM) $(TARGET_PATH)\*.idb,)
	$(if $(wildcard $(TARGET_PATH)\*.pdb), -@$(RM) $(TARGET_PATH)\*.pdb,)
	$(if $(wildcard $(TARGET_PATH)), -@rmdir /S/Q $(TARGET_PATH),)
	@echo Cleaned.
	@echo.