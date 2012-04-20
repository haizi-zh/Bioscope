RM := del
PROJ_NAME:=MMCorePy
PREFIX:=_
POSTFIX:=
EXTENSION :=.pyd
TARGET_NAME := $(PREFIX)$(PROJ_NAME)$(POSTFIX)$(EXTENSION)
CONFIG:=Release
TARGET_PATH := $(CONFIG)
TARGET := $(TARGET_PATH)\$(TARGET_NAME)
PYTHON_TARGET:=$(PROJ_NAME).py

OBJ_FILES := _MMCorePy.o

MM_ROOT:=..\..\mmroot
THIRD_PARTY:=$(MM_ROOT)\3rdpartypublic
IFLAGS:=/I"$(WIN_SDK)\Include" /I"${PYTHON_INCLUDE}" /I"$(PYTHON_INCLUDE)\..\Lib\site-packages\numpy\core\include\numpy"
PREPROCS:=WIN32 _WINDOWS _USRDLL MMCOREPY_WRAP_EXPORTS _WINDLL _MBCS NDEBUG __WIN32__
CFLAGS:=/c /Zi /nologo /W4 /WX- /O2 /Oy- /Gm- /EHa /MD /GS /fp:precise /Zc:wchar_t /Zc:forScope /GR /Gd /wd"4290" /wd"4127" /analyze- /errorReport:queue 
LINK_FLAGS:=/INCREMENTAL:NO /NOLOGO /DLL /DEBUG /SUBSYSTEM:WINDOWS /OPT:REF /OPT:ICF /TLBID:1 /DYNAMICBASE:NO /IMPLIB:"$(TARGET_PATH)\$(PROJ_NAME).lib" /MACHINE:X86 /ERRORREPORT:QUEUE
LIBS:=Iphlpapi.lib mmcorer.lib
PREPROC_FLAGS := $(patsubst %,/D%,$(PREPROCS))
CXX := cl
LIBPATH:=$(PYTHON_INCLUDE)\..\Libs $(MM_ROOT)\micromanager\lib_Win32 $(THIRD_PARTY)\boost\stage\lib
LIB_FLAGS := /LIBPATH:"$(WIN_SDK)\Lib" $(patsubst %,/LIBPATH:%,$(LIBPATH))
LIBS:=Iphlpapi.lib MMCorer.lib

.PHONY: all clean pre-build main-build post-build

all: pre-build main-build post-build

#@vcvars32.bat
pre-build:
	@echo.
	@echo Pre-building...
	$(if $(wildcard $(TARGET_PATH)), , -@mkdir $(subst /,\,$(TARGET_PATH)))
	$(if $(wildcard $(TARGET_PATH)/*.o),-@move /Y "$(TARGET_PATH)\*.o" ./,)
	$(if $(wildcard $(TARGET_PATH)/*.idb),-@move /Y "$(TARGET_PATH)\*.idb" ./,)
	$(if $(wildcard $(TARGET_PATH)/*.pdb),-@move /Y "$(TARGET_PATH)\*.pdb" ./,)
	@echo.
	
main-build: $(TARGET) $(PYTHON_TARGET)

$(PYTHON_TARGET) $(PROJ_NAME)_wrap.cxx: MMCorePy.i
	@echo swig
	swig -python -c++ -o $(PROJ_NAME)_wrap.cxx $<
	@echo.

$(TARGET): $(PREFIX)$(PROJ_NAME)$(POSTFIX).o
	@echo Building target: $@, configuration: $(CONFIG).
	@echo Invoking: Linker (link)
	link $(LINK_FLAGS) /OUT:$(TARGET) $^ $(LIBS) $(LIB_FLAGS)
	@echo Finished building target: $@
	@echo.
	
$(PREFIX)$(PROJ_NAME)$(POSTFIX).o: $(PROJ_NAME)_wrap.cxx
	@echo Building file: $<
	@echo Invoking: C++ Compiler (cl)
	$(CXX) $(CFLAGS) $(PREPROC_FLAGS) $(IFLAGS) /Fo$@ $<
	@echo Finished building: $<, out: $@
	@echo.
	
post-build:
	copy $(TARGET) $(MM_ROOT)\micromanager\bin_Win32
	copy $(PYTHON_TARGET) $(MM_ROOT)\micromanager\bin_Win32
	-@move /Y *.o "$(TARGET_PATH)\\"
	$(if $(wildcard $(TARGET_PATH)\*.idb),-@move /Y *.idb "$(TARGET_PATH)\\",)
	$(if $(wildcard $(TARGET_PATH)\*.pdb),-@move /Y *.pdb "$(TARGET_PATH)\\",)
	@echo.
	
clean:
	@echo Cleaning project...
	$(if $(wildcard $(TARGET_PATH)\*.lib), -$(RM) $(TARGET_PATH)\*.lib,)
	$(if $(wildcard $(TARGET_PATH)\*.pyd), -$(RM) $(TARGET_PATH)\*.pyd,)
	$(if $(wildcard $(TARGET_PATH)\*.o), -$(RM) $(TARGET_PATH)\*.o,)
	$(if $(wildcard *.cxx), -$(RM) *.cxx,)
	-$(RM) MMCorePy.py
	-$(RM) MMCorePy_wrap.h
	$(if $(wildcard $(TARGET_PATH)\*.idb), -$(RM) $(TARGET_PATH)\*.idb,)
	$(if $(wildcard $(TARGET_PATH)\*.pdb), -$(RM) $(TARGET_PATH)\*.pdb,)
	$(if $(wildcard $(TARGET_PATH)), -rmdir /S/Q $(TARGET_PATH),)
	@echo Cleaned.
	@echo.