---
Generator: Umba Brief Scanner
---

# _libs/encoding

- [ ] `[_libs/encoding/encoding.cpp:370]`
  А если продетектированная по BOM кодировка не соответствует заданной
  параметром?

- [ ] `[_libs/encoding/encoding.cpp:386]`
  разобраться, не будет ли precomposed всякие ударения соединять с символом?



# _libs/encoding/compact_enc_det

- [ ] `[_libs/encoding/compact_enc_det/compact_enc_det.cc:46]`
  as of 2007.10.09:
  
  Consider font=TT-BHxxx as user-defined => binary Demote GB18030 if no 8x3x pair
  Map byte2 ascii punct to 0x60, digits to 0x7e, gets them into hires Consider
  removing/ignoring bytes 01-1F to avoid crap pollution Possibly boost declared
  encoding in robust scan googlebot tiny files look for ranges of encodings
  consider tags just as > < within aligned block of 32 flag too few characters in
  postproc (Latin 6 problem) Remove slow scan beyond 16KB Consider removing
  kMostLikelyEncoding or cut it in half

- [ ] `[_libs/encoding/compact_enc_det/compact_enc_det.cc:393]`
  only allocate this if being used

- [ ] `[_libs/encoding/compact_enc_det/compact_enc_det.cc:1200]`
  change this to be per encoding, not permuted

- [ ] `[_libs/encoding/compact_enc_det/compact_enc_det.cc:1721]`
  This should all be folded into postproc-enc-detect.cc

- [ ] `[_libs/encoding/compact_enc_det/compact_enc_det.cc:1858]`
  This should all be folded into postproc-enc-detect.cc

- [ ] `[_libs/encoding/compact_enc_det/compact_enc_det.cc:1946]`
  This should all be folded into postproc-enc-detect.cc

- [ ] `[_libs/encoding/compact_enc_det/compact_enc_det.cc:2181]`
  make this more graceful

- [ ] `[_libs/encoding/compact_enc_det/compact_enc_det.cc:4028]`
  boost subset/superset also Boost(destatep, kRelatedEncoding[best_enc],
  kBestEncBoost);

- [ ] `[_libs/encoding/compact_enc_det/compact_enc_det.cc:4746]`
  also want to boost Shift-JIS here if byte1 is Ax..Dx TEMP printf(" 
  Rescore[%02x] %s +%d\n", high_hash, MyRankedEncName(rankedencoding),
  kGentlePairBoost);



# _libs/encoding/util

- [ ] `[_libs/encoding/util/basictypes.h:34]`
  (mbelshe) Remove these type guards.  These are temporary to avoid conflicts
  with npapi.h.

- [ ] `[_libs/encoding/util/basictypes.h:57]`
  (mbelshe) Remove these type guards.  These are temporary to avoid conflicts
  with npapi.h.



# _libs/encoding/util/encodings

- [ ] `[_libs/encoding/util/encodings/encodings.cc:315]`
  This list could be expanded.  Many other encodings are supersets of 7-bit
  Ascii.  In fact, Japanese JIS and Unicode are the only two encodings that I
  know for a fact should *not* be in this list.

- [ ] `[_libs/encoding/util/encodings/encodings.cc:374]`
  This list could maybe be expanded.  Other encodings may be 8-bit.

- [ ] `[_libs/encoding/util/encodings/encodings.cc:520]`
  Unify MimeEncodingName and EncodingName, or determine why such a unification is
  not possible.

- [ ] `[_libs/encoding/util/encodings/encodings.cc:525]`
  Should this be invalid_encoding_name()?

- [ ] `[_libs/encoding/util/encodings/encodings.cc:860]`
  Check if it is possible to remove the non-standard, non-netscape-use names. It
  is because this routine is used for encoding detections from html meta info.
  Non-standard names may introduce noise on encoding detection.

- [ ] `[_libs/encoding/util/encodings/encodings.cc:865]`
  Unify EncodingNameAliasToEncoding and EncodingFromName, or determine why such a
  unification is not possible.
  ----------------------------------------------------------------------

- [ ] `[_libs/encoding/util/encodings/encodings.h:48]`
  Include a SWIG workaround header file.

- [ ] `[_libs/encoding/util/encodings/encodings.h:55]`
  Make these static const Encoding values instead of macros.

- [ ] `[_libs/encoding/util/encodings/encodings.h:106]`
  the current implementation is likely incomplete.  It would be good to consider
  the full matrix of all pairs of encodings and to fish out all compatible pairs.

- [ ] `[_libs/encoding/util/encodings/encodings.h:151]`
  Get rid of this function. The only special-case we should need to worry about
  are visual encodings. Anything we need to do for all 'RTL' encodings we need to
  do for UTF-8 as well.

- [ ] `[_libs/encoding/util/encodings/encodings.h:165]`
  Get rid of this function. The only special-case we should need to worry about
  are visual encodings.



# _libs/encoding/util/languages

- [ ] `[_libs/encoding/util/languages/languages.cc:69]`
  Although Teragram has two output names "TG_UNKNOWN_LANGUAGE" and "Unknown",
  they are essentially the same. Need to unify them. "un" and "ut" are invented
  by us, not from ISO-639.

- [ ] `[_libs/encoding/util/languages/languages.h:33]`
  Include a header containing swig-compatible enum.

- [ ] `[_libs/encoding/util/languages/languages.h:137]`
  If we want to do anything special with multi-script languages we should create
  new 'languages' for each language+script, as we do for traditional vs.
  simplified Chinese. However most such languages are rare in use and even rarer
  on the web, so this is unlikely to be something we'll be concerned with for a
  while.



# _libs/marty_cpp

- [ ] `[_libs/marty_cpp/marty_cpp.h:690]`
  пока range_error не кидаем, но надо доделать

- [ ] `[_libs/marty_cpp/marty_cpp.h:2296]`
  Надо подумать, править баг и как его править

- [ ] `[_libs/marty_cpp/marty_cpp.h:3659]`
  Нужен каст к underlaying типу, если он задан. Сюда надо будет передавать
  шаблоны и строку underlaying типа

- [ ] `[_libs/marty_cpp/marty_cpp.h:4699]`
  Нужен каст к underlaying типу, если он задан. Сюда надо будет передавать
  шаблоны и строку underlaying типа



# _libs/nlohmann/single_include/nlohmann

- [ ] `[_libs/nlohmann/single_include/nlohmann/json.hpp:11683]`
  (niels): may we ignore N here?



# _libs/nlohmann/tests/src

- [ ] `[_libs/nlohmann/tests/src/unit-element_access2.cpp:1493]`
  (falbrechtskirchinger) merge with the other test case; clean up

- [ ] `[_libs/nlohmann/tests/src/unit-udt.cpp:208]`
  (nlohmann) test exceptions



# _libs/nlohmann/tests/thirdparty/doctest

- [ ] `[_libs/nlohmann/tests/thirdparty/doctest/doctest.h:577]`
  - optimizations - like not deleting memory unnecessarily in operator= and etc.
  - resize/reserve/clear - replace - back/front - iterator stuff - find & friends
  - push_back/pop_back - assign/insert/erase - relational operators as free
  functions - taking const char* as one of the params

- [ ] `[_libs/nlohmann/tests/thirdparty/doctest/doctest.h:1401]`
  Why do we need this? (To remove NOLINT)

- [ ] `[_libs/nlohmann/tests/thirdparty/doctest/doctest.h:3596]`
  figure out if this is indeed necessary/correct - seems like either there still
  could be a race or that there wouldn't be a race even if using the context
  directly

- [ ] `[_libs/nlohmann/tests/thirdparty/doctest/doctest.h:3911]`
  under DOCTEST_MSVC: does the comparison need strnicmp() to work with drive
  letter capitalization?

- [ ] `[_libs/nlohmann/tests/thirdparty/doctest/doctest.h:4894]`
  integration with XCode and other IDEs

- [ ] `[_libs/nlohmann/tests/thirdparty/doctest/doctest.h:5730]`
  - log_message() - respond to queries - honor remaining options - more
  attributes in tags

- [ ] `[_libs/nlohmann/tests/thirdparty/doctest/doctest.h:6591]`
  change this to use std::stoi or something else! currently it uses undefined
  behavior - assumes '0' on failed parse...

- [ ] `[_libs/nlohmann/tests/thirdparty/doctest/doctest.h:6885]`
  check if there is nothing in reporters_currently_used



# _libs/nlohmann/tests/thirdparty/Fuzzer

- [ ] `[_libs/nlohmann/tests/thirdparty/Fuzzer/FuzzerTraceState.cpp:141]`
  std::set is too inefficient, need to have a custom DS here.

- [ ] `[_libs/nlohmann/tests/thirdparty/Fuzzer/FuzzerUtilWindows.cpp:161]`
  make this implementation more efficient.



# _libs/nlohmann/tests/thirdparty/Fuzzer/test

- [ ] `[_libs/nlohmann/tests/thirdparty/Fuzzer/test/MemcmpTest.cpp:11]`
  check other sizes.

- [ ] `[_libs/nlohmann/tests/thirdparty/Fuzzer/test/StrncmpTest.cpp:13]`
  check other sizes.



# _libs/umba

- [ ] `[_libs/umba/enum_helpers.h:475]`
  Сделать так же, как для QString

- [ ] `[_libs/umba/enum_helpers.h:483]`
  Сделать так же, как для QString

- [ ] `[_libs/umba/filesys_scanners.h:31]`
  Надо бы переименовать в umba::filesys::scanners

- [ ] `[_libs/umba/filesys_scanners.h:285]`
  Нужно что-то решать с отсутствующим расширением

- [ ] `[_libs/umba/filesys_scanners.h:503]`
  Нужно что-то решать с отсутствующим расширением

- [ ] `[_libs/umba/format_message.h:57]`
  Надо подумать на тему замены десятичного разделителя и разделителя разрядов

- [ ] `[_libs/umba/format_message.h:303]`
  Не реализовано ! Десятичный разделитель - между целой и дробной частью

- [ ] `[_libs/umba/format_message.h:307]`
  Не реализовано ! Разделитель груп разрядов и размер группы

- [ ] `[_libs/umba/format_message.h:311]`
  Не реализовано ! Установка нац особенностей форматирования десятичных чисел,
  делает decSep и decGroup

- [ ] `[_libs/umba/regex_helpers.h:197]`
  Если не обрезать, то регулярки в GCC 7.3 падают Чет не работает, а обрезать в
  прикладухе - работает

- [ ] `[_libs/umba/regex_helpers.h:232]`
  Если не обрезать, то регулярки в GCC 7.3 падают Чет не работает, а обрезать в
  прикладухе - работает

- [ ] `[_libs/umba/string_plus.h:2099]`
  Надо подумать, править баг и как его править

- [ ] `[_libs/umba/string_plus.h:2156]`
  Надо подумать, править баг и как его править

- [ ] `[_libs/umba/utf_impl.h:772]`
  Надо бы сделать: UTF-32 из string UTF-32 из wstring string  из UTF-32 wstring
  из UTF-32



# _libs/umba/internal

- [ ] `[_libs/umba/internal/filesys.h:591]`
  Посмотреть "Именование файлов, путей и пространств имен" -
  https://learn.microsoft.com/ru-ru/windows/win32/fileio/naming-a-file

- [ ] `[_libs/umba/internal/filesys.h:1597]`
  При возникновении исключения hFind утекает. Надо бы переделать, и такие моменты
  надо бы везде это проверить

- [ ] `[_libs/umba/internal/filesys.h:1656]`
  При возникновении исключения hFind утекает. Надо бы везде это проверить HANDLE
  hFind = ::FindFirstFileA( umba::filename::prepareForNativeUsage(path).c_str(),
  &fndData );

- [ ] `[_libs/umba/internal/filesys.h:2030]`
  Переделать !!!
  https://stackoverflow.com/questions/2910377/get-home-directory-in-linux

- [ ] `[_libs/umba/internal/winerror_org.h:34314]`
  Add DXG Win32 errors here
  
  Common errors {0x2000..0x20ff}
  
  MessageId: ERROR_GRAPHICS_NOT_EXCLUSIVE_MODE_OWNER
  
  MessageText:
  
  Exclusive mode ownership is needed to create unmanaged primary allocation.



# _libs/umba/warnings

- [ ] `[_libs/umba/warnings/disable_unsafe_conversion.h:15]`
  Проверить на 8/9/10/11/12 ой версии GCC - на 13ой точно есть



# _libs/yaml-cpp/src

- [ ] `[_libs/yaml-cpp/src/emitter.cpp:317]`
  if we were writing null, and we wanted it blank, we wouldn't want a space

- [ ] `[_libs/yaml-cpp/src/emitterstate.cpp:139]`
  Create move constructors for settings types to simplify transfer

- [ ] `[_libs/yaml-cpp/src/emitterutils.cpp:203]`
  check for non-printable characters?

- [ ] `[_libs/yaml-cpp/src/emitterutils.cpp:216]`
  check for non-printable characters?

- [ ] `[_libs/yaml-cpp/src/parser.cpp:94]`
  warning on major == 1, minor > 2?

- [ ] `[_libs/yaml-cpp/src/ptr_vector.h:17]`
  This class is no longer needed





# _libs/yaml-cpp/src/contrib

- [ ] `[_libs/yaml-cpp/src/contrib/dragonbox.h:372]`
  static overflow guard against intermediate computations.



# _libs/yaml-cpp/test

- [ ] `[_libs/yaml-cpp/test/specexamples.h:289]`
  5.1 - 5.2 BOM

- [ ] `[_libs/yaml-cpp/test/specexamples.h:322]`
  5.9 directive

- [ ] `[_libs/yaml-cpp/test/specexamples.h:323]`
  5.10 reserved indicator



# _libs/yaml-cpp/test/googletest-1.13.0/googlemock/test

- [ ] `[_libs/yaml-cpp/test/googletest-1.13.0/googlemock/test/gmock-pp_test.cc:73]`
  (iserna): The following asserts fail in --config=lexan.



# _libs/yaml-cpp/test/googletest-1.13.0/googletest/src

- [ ] `[_libs/yaml-cpp/test/googletest-1.13.0/googletest/src/gtest-printers.cc:281]`
  (dcheng): Consider making this delegate to PrintCharAndCodeTo() as well.

- [ ] `[_libs/yaml-cpp/test/googletest-1.13.0/googletest/src/gtest.cc:804]`
  (b/214626361): Fail on multiple '-' characters For the moment to preserve old
  behavior we concatenate the rest of the string parts with `-` as separator to
  generate the negative filter.

- [ ] `[_libs/yaml-cpp/test/googletest-1.13.0/googletest/src/gtest.cc:2327]`
  (jdesprez): Merge the two getReserved attributes once skip is improved



# _libs/yaml-cpp/test/googletest-1.13.0/googletest/test

- [ ] `[_libs/yaml-cpp/test/googletest-1.13.0/googletest/test/gtest_unittest.cc:7608]`
  (ofats): it should be testing assignment operator of FlatTuple, not its
  elements

- [ ] `[_libs/yaml-cpp/test/googletest-1.13.0/googletest/test/gtest_unittest.cc:7624]`
  (ofats): it should be testing assignment operator of FlatTuple, not its
  elements



# _libs/yaml-cpp/test/integration

- [ ] `[_libs/yaml-cpp/test/integration/handler_spec_test.cpp:639]`
  5.1 - 5.2 BOM

- [ ] `[_libs/yaml-cpp/test/integration/handler_spec_test.cpp:720]`
  5.9 directive

- [ ] `[_libs/yaml-cpp/test/integration/handler_spec_test.cpp:721]`
  5.10 reserved indicator

- [ ] `[_libs/yaml-cpp/test/integration/handler_spec_test.cpp:1002]`
  Implement

- [ ] `[_libs/yaml-cpp/test/integration/handler_spec_test.cpp:1025]`
  should we reject this one (since !h! is not declared)?

- [ ] `[_libs/yaml-cpp/test/integration/node_spec_test.cpp:240]`
  2.19 - 2.22 schema tags

- [ ] `[_libs/yaml-cpp/test/integration/node_spec_test.cpp:395]`
  5.1 - 5.2 BOM

- [ ] `[_libs/yaml-cpp/test/integration/node_spec_test.cpp:428]`
  assert tag

- [ ] `[_libs/yaml-cpp/test/integration/node_spec_test.cpp:446]`
  5.9 directive

- [ ] `[_libs/yaml-cpp/test/integration/node_spec_test.cpp:447]`
  5.10 reserved indicator

- [ ] `[_libs/yaml-cpp/test/integration/node_spec_test.cpp:679]`
  check tags (but we probably will say these are valid, I think)

- [ ] `[_libs/yaml-cpp/test/integration/node_spec_test.cpp:698]`
  should we reject this one (since !h! is not declared)?

- [ ] `[_libs/yaml-cpp/test/integration/node_spec_test.cpp:707]`
  check tags. How?

