RM := del
PROJ_NAME := MMCoreJ_wrap
PREFIX := 
POSTFIX := 
EXTENSION := .dll
JAR_FILE := MMCoreJ.jar
TARGET_NAME := $(PREFIX)$(PROJ_NAME)$(POSTFIX)$(EXTENSION)
ifeq ($(DEBUG), 1)
	CONFIG := Debug
else
	CONFIG := Release
endif
TARGET_PATH := $(CONFIG)
TARGET := $(TARGET_PATH)\$(TARGET_NAME)

OBJ_FILES := MMCoreJ_wrap.o

MM_ROOT:=..\..\mmroot
THIRD_PARTY:=$(MM_ROOT)\3rdpartypublic
IFLAGS:=/I"$(WIN_SDK)\Include" /I"${JAVA_HOME}\include" /I"$(JAVA_HOME)\include\win32"
PREPROCS:=WIN32 _WINDOWS _USRDLL MMCOREJ_WRAP_EXPORTS _WINDLL _MBCS
ifeq ($(DEBUG), 1)
	PREPROCS += _DEBUG
	CFLAGS:=/c /ZI /nologo /W4 /WX- /Od /Oy- /Gm /EHa /RTC1 /MDd /GS /fp:precise /Zc:wchar_t /Zc:forScope /GR /Gd /wd"4290" /analyze- /errorReport:queue
	LINK_FLAGS:=/INCREMENTAL /NOLOGO /DLL /DEBUG /SUBSYSTEM:WINDOWS /TLBID:1 /DYNAMICBASE:NO /IMPLIB:"$(TARGET_PATH)\$(PROJ_NAME).lib" /MACHINE:X86 /ERRORREPORT:QUEUE
	LIBS:=Iphlpapi.lib mmcored.lib 
else
	PREPROCS += NDEBUG
	CFLAGS :=/c /Zi /nologo /W4 /WX- /O2 /Oy- /Gm- /EHa /MD /GS /fp:precise /Zc:wchar_t /Zc:forScope /GR /Gd /wd"4290" /analyze- /errorReport:queue
	LINK_FLAGS:=/INCREMENTAL:NO /NOLOGO /DLL /DEBUG /SUBSYSTEM:WINDOWS /OPT:REF /OPT:ICF /TLBID:1 /DYNAMICBASE:NO /IMPLIB:"$(TARGET_PATH)\$(PROJ_NAME).lib" /MACHINE:X86 /ERRORREPORT:QUEUE
	LIBS:=Iphlpapi.lib mmcorer.lib
endif
PREPROC_FLAGS := $(patsubst %,/D%,$(PREPROCS))
CXX := cl
LIBPATH:=$(MM_ROOT)\micromanager\lib_Win32 $(THIRD_PARTY)\boost\stage\lib
LIB_FLAGS := /LIBPATH:"$(WIN_SDK)\Lib" $(patsubst %,/LIBPATH:%,$(LIBPATH))

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
	
main-build: $(TARGET) $(JAR_FILE)

$(JAR_FILE) $(PROJ_NAME).cxx: MMCoreJ.i TaggedImage.java
	@echo swig
	-$(RM) mmcorej\*.java
	swig -c++ -java -package mmcorej -outdir mmcorej -module MMCoreJ MMCoreJ.i
	@copy TaggedImage.java mmcorej
	-$(RM) mmcorej\*.class
	javac -source 5 mmcorej/*.java ../mmstudio/src/org/json/*.java
	jar cvf $(JAR_FILE) mmcorej/*.class
	@echo.

$(TARGET): $(OBJ_FILES)
	@echo Building target: $@, configuration: $(CONFIG).
	@echo Invoking: Linker (link)
	link $(LINK_FLAGS) /OUT:$(TARGET) $^ $(LIBS) $(LIB_FLAGS)
	@echo Finished building target: $@
	@echo.
	
$(OBJ_FILES): %.o: %.cxx
	@echo Building file: $<
	@echo Invoking: C++ Compiler (cl)
	$(CXX) $(CFLAGS) $(PREPROC_FLAGS) $(IFLAGS) /Fo$@ $<
	@echo Finished building: $<, out: $@
	@echo.
	
post-build:
	copy $(JAR_FILE) $(MM_ROOT)\micromanager\bin_Win32
	copy $(TARGET) $(MM_ROOT)\micromanager\bin_Win32
	-@move /Y *.o "$(TARGET_PATH)\\"
	$(if $(wildcard $(TARGET_PATH)\*.idb),-@move /Y *.idb "$(TARGET_PATH)\\",)
	$(if $(wildcard $(TARGET_PATH)\*.pdb),-@move /Y *.pdb "$(TARGET_PATH)\\",)
	@echo.
	
clean:
	@echo Cleaning project...
	$(if $(wildcard $(TARGET_PATH)\*.lib), -@$(RM) $(TARGET_PATH)\*.lib,)
	$(if $(wildcard $(TARGET_PATH)\*.dll), -@$(RM) $(TARGET_PATH)\*.dll,)
	$(if $(wildcard $(TARGET_PATH)\*.o), -@$(RM) $(TARGET_PATH)\*.o,)
	$(if $(wildcard *.cxx), -@$(RM) *.cxx,)
	-@$(RM) MMCoreJ_wrap.h
	-@$(RM) mmcorej\*.java
	-@$(RM) mmcorej\*.class
	$(if $(wildcard $(TARGET_PATH)\*.idb), -@$(RM) $(TARGET_PATH)\*.idb,)
	$(if $(wildcard $(TARGET_PATH)\*.pdb), -@$(RM) $(TARGET_PATH)\*.pdb,)
	$(if $(wildcard $(TARGET_PATH)), -@rmdir /S/Q $(TARGET_PATH),)
	@echo Cleaned.
	@echo.