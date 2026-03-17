@set UINT8=--underlaying-type=std::uint8_t
@set UINT16=--underlaying-type=std::uint16_t
@set UINT32=--underlaying-type=std::uint32_t
@set INT=--underlaying-type=int

@set HEX2=--hex-width=2
@set HEX4=--hex-width=4
@set HEX6=--hex-width=6
@set HEX8=--hex-width=8

@rem set EXTRA=--enum-flags=extra
@rem same but more detailed
@set FLAGENUM_EXTRA=--enum-flags=serialize-set,deserialize-set

@set FLAGS=--enum-flags=flags
@set DECL=--enum-flags=type-decl
@set CLS=--enum-flags=enum-class
@set DECLCLS=--enum-flags=type-decl,enum-class
@set FMTHEX=--enum-flags=fmt-hex
@set FMTDEC=--enum-flags=fmt-dec
@set SERIALIZE=--enum-flags=serialize,deserialize,lowercase

@set TPL_OVERRIDE=--override-template-parameter=EnumNameFormat:$(ENAMNAME)
@set GEN_OPTS=--enum-values-style=CamelStyle

@set VALUES_CAMEL=--enum-values-style=CamelStyle
@set SERIALIZE_CAMEL=--serialize-style=CamelStyle
@set VALUES_PASCAL=--enum-values-style=PascalStyle
@set SERIALIZE_PASCAL=--serialize-style=PascalStyle
@set SERIALIZE_HYPHEN=--serialize-style=HyphenStyle


@set SNIPPETOPTIONS_GEN_FLAGS=--enum-flags=0 --enum-flags=type-decl,serialize,deserialize,lowercase,enum-class,fmt-hex %VALUES_CAMEL% %SERIALIZE_PASCAL% --enum-serialize-style=All


umba-enum-gen %GEN_OPTS% %HEX2% %TPL_OVERRIDE% %SNIPPETOPTIONS_GEN_FLAGS% ^
    %UINT32% %HEX4% -E=CodeLanguageMarker            -F=@CodeLanguageMarker.txt           ^
    %UINT32% %HEX4% -E=ComparisonType                -F=@ComparisonType.txt               ^
    %UINT32% %HEX4% -E=FenceStyle                    -F=@FenceStyle.txt                   ^
    %UINT32% %HEX4% -E=FilenameDecorationStyle       -F=@FilenameDecorationStyle.txt      ^
    %UINT32% %HEX4% -E=FilenameDecorationType        -F=@FilenameDecorationType.txt       ^
    %UINT32% %HEX4% -E=ResultCode                    -F=@ResultCode.txt                   ^
    %UINT32% %HEX4% -E=SortOrder                     -F=@SortOrder.txt                    ^
    %SERIALIZE_HYPHEN%                                                                    ^
..\enums.h

@rem Что-то пошло не так и файл генерится в текущий каталог
if exist %~dp0enums.h copy /Y %~dp0enums.h %~dp0..\enums.h
if exist %~dp0enums.h del %~dp0enums.h

