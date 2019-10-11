# NMake makefile
!include BuildConfiguration.mk
temp = $(sources:src/=build/)
temp2 = $(sources:src/=)
build_objs = $(temp:.c=.o)
files = $(temp2:.c=)

debug:
	echo $(build_deps)

clean:
    del bin\* /Q

bin\debug-msvc.exe: $(build_objs)
    link /OUT:$@ $(build_objs)

!IF [NMakeHelperGenerator.bat "$(files)" "$(msvc_flags)" > .NMakeHelper.mk]
!ENDIF

!INCLUDE .NMakeHelper.mk