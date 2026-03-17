---
Generator: Umba Brief Scanner
---

# _libs/encoding/compact_enc_det

- `[_libs/encoding/compact_enc_det/compact_enc_det.cc:1983]`
  initial pointer is to FRONT of string, not back

- `[_libs/encoding/compact_enc_det/compact_enc_det.cc:2063]`
  Encoding probabilites are all zero at this point

- `[_libs/encoding/compact_enc_det/compact_enc_det.cc:2520]`
  this requires at least one lower AND one upper AND one digit to pass

- `[_libs/encoding/compact_enc_det/compact_enc_det.cc:5184]`
  destate and scoped_debug_data have exactly the same scope All other
  FLAGS_enc_detect_detail tests use destate.debug_data != NULL

- `[_libs/encoding/compact_enc_det/compact_enc_det.cc:5247]`
  Encoding probabilites are all zero at this point

- `[_libs/encoding/compact_enc_det/compact_enc_det.cc:5256]`
  probabilities up to this point are subject to derating for small numbers of
  bigrams. Probability changes after this point are not derated.

- `[_libs/encoding/compact_enc_det/compact_enc_det_unittest.cc:1986]`
  Unicode handled differently because of embedded NUL

- `[_libs/encoding/compact_enc_det/compact_enc_det_unittest.cc:4745]`
  strlen will not work for UTF-16 and UTF-32 strings with embedded NUL



# _libs/encoding/util

- `[_libs/encoding/util/basictypes.h:49]`
  unsigned types are DANGEROUS in loops and other arithmetical places.  Use the
  signed types unless your variable represents a bit pattern (eg a hash value) or
  you really need the extra bit.  Do NOT use 'unsigned' to express "this value
  should always be positive"; use assertions for this.

- `[_libs/encoding/util/basictypes.h:115]`
  that the function doesn't need an implementation, as we only use its type.



# _libs/encoding/util/encodings

- `[_libs/encoding/util/encodings/encodings.cc:48]`
  that the preferred MIME name may differ slightly from the official IANA name:
  i.e. ISO-8859-1 vs. ISO_8859-1:1987

- `[_libs/encoding/util/encodings/encodings.cc:216]`
  that MimeEncodingName() returns "Shift_JIS", since KDDI uses "Shift_JIS" in
  HTTP headers and email messages.

- `[_libs/encoding/util/encodings/encodings.cc:459]`
  that despite an RFC to the contrary, ARABIC_ENCODING (ISO-8859-6) is NOT
  visual.

- `[_libs/encoding/util/encodings/encodings.cc:576]`
  that MimeEncodingName() can return the same mime encoding name for different
  encoding enums like JAPANESE_SHIFT_JIS and KDDI_SHIFT_JIS.  In that case, the
  encoding enum first seen will be the value for the encoding name in the map.

- `[_libs/encoding/util/encodings/encodings.h:39]`
  The Encoding enum must always start at 0. This assumption has been made and
  used.

- `[_libs/encoding/util/encodings/encodings.h:146]`
  that the name of this function is somewhat misleading. There is nothing "right
  to left" about these encodings. They merely contain code points for characters
  in RTL languages such as Hebrew and Arabic. But this is also true for UTF-8.



# _libs/encoding/util/languages

- `[_libs/encoding/util/languages/languages.h:122]`
  Technically we're talking about scripts, not languages. There are languages
  that can be written in more than one script. Examples: - Kurdish and Azeri
  ('AZERBAIJANI') can be written left-to-right in Latin or Cyrillic script, and
  right-to-left in Arabic script. - Sindhi and Punjabi are written in different
  scripts, depending on region and dialect. - Turkmen used an Arabic script
  historically, but not any more. - Pashto and Uyghur can use Arabic script, but
  use a Roman script on the Internet. - Kashmiri and Urdu are written either with
  Arabic or Devanagari script.
  
  This function only returns true for languages that are always, unequivocally
  written in right-to-left script.

- `[_libs/encoding/util/languages/languages.h:150]`
  See important notes under IsRightToLeftLanguage(...).
  
  This function returns true for languages that *may* appear on the web in a
  right-to-left script, even if they may also appear in a left-to-right script.
  
  This function should typically be used in cases where doing some work on
  left-to-right text would be OK (usually a no-op), and this function is used
  just to cut down on unnecessary work on regular, LTR text.

- `[_libs/encoding/util/languages/languages.h:250]`
  See the note below about the codes for Chinese languages.

- `[_libs/encoding/util/languages/languages.h:265]`
  CHINESE LANGUAGE CODES
  
  There are three functions that return codes for Chinese languages.
  LanguageCode(lang) and LanguageCodeWithDialects(lang) are defined here.
  LanguageCode(lang, encoding) is defined in i18n/encodings.lang_enc.h. The
  following list shows the different results.
  
  LanguageCode(CHINESE) returns "zh" LanguageCode(CHINESE_T) returns "zh-TW".
  
  LanguageCodeWithDialects(CHINESE) returns "zh-CN".
  LanguageCodeWithDialects(CHINESE_T) returns "zh-TW".
  
  LanguageCode(CHINESE_T, <any encoding>) returns "zh-TW". LanguageCode(CHINESE,
  CHINESE_BIG5) returns "zh-TW". LanguageCode(CHINESE, <any other encoding>)
  returns "zh-CN".
  
  --------------------------------------------

- `[_libs/encoding/util/languages/languages.pb.h:187]`
  If you add a language, you will break a unittest. See the note at the top of
  this enum.



# _libs/nlohmann/single_include/nlohmann

- `[_libs/nlohmann/single_include/nlohmann/json.hpp:3275]`
  that SeqSize == sizeof...(Ints). It's passed explicitly for efficiency.

- `[_libs/nlohmann/single_include/nlohmann/json.hpp:3586]`
  to maintainers:
  
  Every trait in this file expects a non-CV-qualified type. The only exceptions
  are in the 'aliases for detected' section (i.e., those of the form:
  decltype(T::member_function(std::declval<T>())))
  
  In this case, T has to be properly CV-qualified to constraint the function
  arguments (e.g., to_json(BasicJsonType&, const T&))

- `[_libs/nlohmann/single_include/nlohmann/json.hpp:12525]`
  , no reader for UBJSON binary types is implemented because they do not exist

- `[_libs/nlohmann/single_include/nlohmann/json.hpp:15010]`
  at performs range check

- `[_libs/nlohmann/single_include/nlohmann/json.hpp:15025]`
  at performs range check

- `[_libs/nlohmann/single_include/nlohmann/json.hpp:15117]`
  at performs range check

- `[_libs/nlohmann/single_include/nlohmann/json.hpp:15132]`
  at performs range check

- `[_libs/nlohmann/single_include/nlohmann/json.hpp:18389]`
  Since rest and delta share the same exponent e, it suffices to compare the
  significands.

- `[_libs/nlohmann/single_include/nlohmann/json.hpp:18429]`
  No decimal point is generated: the exponent is adjusted instead.
  
  p2 actually represents the fraction
  
  p2 * 2^e = p2 / 2^-e = d[-1] / 10^1 + d[-2] / 10^2 + ...
  
  Now generate the digits d[-m] of p1 from left to right (m = 1,2,...)
  
  p2 * 2^e = d[-1]d[-2]...d[-m] * 10^-m + 10^-m * (d[-m-1] / 10^1 + d[-m-2] /
  10^2 + ...)
  
  using
  
  10^m * p2 = ((10^m * p2) div 2^-e) * 2^-e + ((10^m * p2) mod 2^-e) = (         
  d) * 2^-e + (                   r)
  
  or 10^m * p2 * 2^e = d + r * 2^e
  
  i.e.
  
  M+ = buffer + p2 * 2^e = buffer + 10^-m * (d + r * 2^e) = (buffer * 10^m + d) *
  10^-m + 10^-m * r * 2^e
  
  and stop as soon as 10^-m * r * 2^e <= delta * 2^e

- `[_libs/nlohmann/single_include/nlohmann/json.hpp:18580]`
  that this does not mean that Grisu2 always generates the shortest possible
  number in the interval (m-, m+).

- `[_libs/nlohmann/single_include/nlohmann/json.hpp:20026]`
  that offset is the distance from the start of the vector to first. We will need
  this later.

- `[_libs/nlohmann/single_include/nlohmann/json.hpp:23712]`
  parentheses around operands are necessary; see
  https://github.com/nlohmann/json/issues/1530

- `[_libs/nlohmann/single_include/nlohmann/json.hpp:25039]`
  erase performs range check



# _libs/nlohmann/tests/src

- `[_libs/nlohmann/tests/src/unit-bjdata.cpp:3486]`
  the float has been replaced by a double

- `[_libs/nlohmann/tests/src/unit-bjdata.cpp:3495]`
  the float has been replaced by a double

- `[_libs/nlohmann/tests/src/unit-bjdata.cpp:3504]`
  the float has been replaced by a double

- `[_libs/nlohmann/tests/src/unit-bjdata.cpp:3591]`
  the floats have been replaced by doubles

- `[_libs/nlohmann/tests/src/unit-bjdata.cpp:3607]`
  the floats have been replaced by doubles

- `[_libs/nlohmann/tests/src/unit-bjdata.cpp:3622]`
  the floats have been replaced by doubles

- `[_libs/nlohmann/tests/src/unit-bjdata.cpp:3640]`
  the floats have been replaced by doubles

- `[_libs/nlohmann/tests/src/unit-bjdata.cpp:3654]`
  the floats have been replaced by doubles

- `[_libs/nlohmann/tests/src/unit-bjdata.cpp:3667]`
  the floats have been replaced by doubles

- `[_libs/nlohmann/tests/src/unit-class_parser.cpp:321]`
  [,] and {,} are not allowed

- `[_libs/nlohmann/tests/src/unit-ubjson.cpp:2186]`
  the float has been replaced by a double

- `[_libs/nlohmann/tests/src/unit-ubjson.cpp:2195]`
  the float has been replaced by a double

- `[_libs/nlohmann/tests/src/unit-ubjson.cpp:2204]`
  the float has been replaced by a double

- `[_libs/nlohmann/tests/src/unit-ubjson.cpp:2291]`
  the floats have been replaced by doubles

- `[_libs/nlohmann/tests/src/unit-ubjson.cpp:2307]`
  the floats have been replaced by doubles

- `[_libs/nlohmann/tests/src/unit-ubjson.cpp:2322]`
  the floats have been replaced by doubles

- `[_libs/nlohmann/tests/src/unit-ubjson.cpp:2340]`
  the floats have been replaced by doubles

- `[_libs/nlohmann/tests/src/unit-ubjson.cpp:2354]`
  the floats have been replaced by doubles

- `[_libs/nlohmann/tests/src/unit-ubjson.cpp:2367]`
  the floats have been replaced by doubles

- `[_libs/nlohmann/tests/src/unit-unicode1.cpp:165]`
  1112064 = 0x1FFFFF code points - 2048 invalid values between 0xD800 and 0xDFFF.



# _libs/nlohmann/tests/thirdparty/Fuzzer

- `[_libs/nlohmann/tests/thirdparty/Fuzzer/FuzzerInterface.h:12]`
  the libFuzzer interface is thin and in the majority of cases you should not
  include this file into your target. In 95% of cases all you need is to define
  the following function in your file: extern "C" int
  LLVMFuzzerTestOneInput(const uint8_t *Data, size_t Size);

- `[_libs/nlohmann/tests/thirdparty/Fuzzer/FuzzerUtilDarwin.cpp:81]`
  Do not introduce any new `return` statements past this point. It is important
  that `ActiveThreadCount` always be decremented when leaving this function.



# _libs/umba

- `[_libs/umba/exception.h:255]`
  - required to be using namespace ::umba::omanip or using namespace std::iomanip
  declared before using this macro

- `[_libs/umba/filecache.h:16]`
  Not safe for threading



# _libs/umba/internal

- `[_libs/umba/internal/winerror_org.h:22986]`
  This assumes that WIN32 errors fall in the range -32k to 32k.
  
  Define bits here so macros are guaranteed to work

- `[_libs/umba/internal/winerror_org.h:31141]`
  that additional FACILITY_SSPI errors are in issperr.h
  
  ****************** FACILITY_CERT ******************
  
  MessageId: TRUST_E_PROVIDER_UNKNOWN
  
  MessageText:
  
  Unknown trust provider.



# _libs/yaml-cpp/src

- `[_libs/yaml-cpp/src/regeximpl.h:100]`
  the convention MatchOp*<Source> is that we can assume IsSourceValid(source). So
  we do all our checks *before* we call these functions

- `[_libs/yaml-cpp/src/regeximpl.h:145]`
  'AND' is a little funny, since we may be required to match things of different
  lengths. If we find a match, we return the length of the FIRST entry on the
  list.

- `[_libs/yaml-cpp/src/regeximpl.h:176]`
  Match, not MatchUnchecked because we need to check validity after the offset

- `[_libs/yaml-cpp/src/scanner.cpp:70]`
  what's left are the unverified tokens

- `[_libs/yaml-cpp/src/scanscalar.h:50]`
  strip means kill all, clip means keep at most one, keep means keep all be
  seeing indentation spaces

- `[_libs/yaml-cpp/src/simplekey.cpp:15]`
  pIndent will *not* be garbage here; we "garbage collect" them so we can always
  refer to them



# _libs/yaml-cpp/test

- `[_libs/yaml-cpp/test/specexamples.h:673]`
  we don't check (on purpose) the >1K chars for an implicit key



# _libs/yaml-cpp/test/googletest-1.13.0/googlemock/src

- `[_libs/yaml-cpp/test/googletest-1.13.0/googlemock/src/gmock-matchers.cc:179]`
  that left_[ilhs] is the only element of left_ that TryAugment will potentially
  transition from kUnused to another value. Any other left_ element holding
  kUnused before TryAugment will be holding it when TryAugment returns.



# _libs/yaml-cpp/test/googletest-1.13.0/googlemock/test

- `[_libs/yaml-cpp/test/googletest-1.13.0/googlemock/test/gmock-internal-utils_test.cc:466]`
  that each inner implementation layer will +1 the number to remove itself from
  the trace. This means that the value is a little higher than expected, but
  close enough.

- `[_libs/yaml-cpp/test/googletest-1.13.0/googlemock/test/gmock-matchers-containers_test.cc:446]`
  that the matcher expects DerivedStruct but we say AStruct inside Field().

- `[_libs/yaml-cpp/test/googletest-1.13.0/googlemock/test/gmock-matchers-containers_test.cc:552]`
  that the matcher expects DerivedStruct but we say AStruct inside Field().

- `[_libs/yaml-cpp/test/googletest-1.13.0/googlemock/test/gmock-port_test.cc:38]`
  if this file is left without tests for some reason, put a dummy test here to
  make references to symbols in the gtest library and avoid 'undefined symbol'
  linker errors in gmock_main:



# _libs/yaml-cpp/test/googletest-1.13.0/googletest/samples

- `[_libs/yaml-cpp/test/googletest-1.13.0/googletest/samples/sample6_unittest.cc:64]`
  that we test an implementation via the base interface instead of the actual
  implementation class.  This is important for keeping the tests close to the
  real world scenario, where the implementation is invoked via the base
  interface.  It avoids got-yas where the implementation class has a method that
  shadows a method with the same name (but slightly different argument types) in
  the base interface, for example.



# _libs/yaml-cpp/test/googletest-1.13.0/googletest/src

- `[_libs/yaml-cpp/test/googletest-1.13.0/googletest/src/gtest-death-test.cc:650]`
  to distinguish Win32 API calls from the local method and function calls, the
  former are explicitly resolved in the global namespace.

- `[_libs/yaml-cpp/test/googletest-1.13.0/googletest/src/gtest-filepath.cc:381]`
  that "\\Host\Share" does not contain a redundancy on Windows!

- `[_libs/yaml-cpp/test/googletest-1.13.0/googletest/src/gtest-internal-inl.h:1141]`
  that Google Test current only report elapsed time for each test iteration, not
  for the entire test program.

- `[_libs/yaml-cpp/test/googletest-1.13.0/googletest/src/gtest-internal-inl.h:1162]`
  that "event=TestCaseStart" is a wire format and has to remain "case" for
  compatibility

- `[_libs/yaml-cpp/test/googletest-1.13.0/googletest/src/gtest-internal-inl.h:1168]`
  that "event=TestCaseEnd" is a wire format and has to remain "case" for
  compatibility

- `[_libs/yaml-cpp/test/googletest-1.13.0/googletest/src/gtest-port.cc:995]`
  that FormatCompilerIndependentFileLocation() does NOT append colon to the file
  location it produces, unlike FormatFileLocation().

- `[_libs/yaml-cpp/test/googletest-1.13.0/googletest/src/gtest-port.cc:1058]`
  Android applications are expected to call the framework's
  Context.getExternalStorageDirectory() method through JNI to get the location of
  the world-writable SD Card directory. However, this requires a Context handle,
  which cannot be retrieved globally from native code. Doing so also precludes
  running the code as part of a regular standalone executable, which doesn't run
  in a Dalvik process (e.g. when running it through 'adb shell').
  
  The location /data/local/tmp is directly accessible from native code. '/sdcard'
  and other variants cannot be relied on, as they are not guaranteed to be
  mounted, or may have a delay in mounting.

- `[_libs/yaml-cpp/test/googletest-1.13.0/googletest/src/gtest-port.cc:1368]`
  that this is meant to be called at the call site so it does not check that the
  flag is 'output' In essence this checks an env variable called XML_OUTPUT_FILE
  and if it is set we prepend "xml:" to its value, if it not set we return ""

- `[_libs/yaml-cpp/test/googletest-1.13.0/googletest/src/gtest.cc:796]`
  `SplitString` always returns a non-empty container.

- `[_libs/yaml-cpp/test/googletest-1.13.0/googletest/src/gtest.cc:1674]`
  that the above two checks will both fail if either val1 or val2 is NaN, as the
  IEEE floating-point standard requires that any predicate involving a NaN must
  return false.

- `[_libs/yaml-cpp/test/googletest-1.13.0/googletest/src/gtest.cc:2631]`
  The user code can affect the way in which Google Test handles exceptions by
  setting GTEST_FLAG(catch_exceptions), but only before RUN_ALL_TESTS() starts.
  It is technically possible to check the flag after the exception is caught and
  either report or re-throw the exception based on the flag's value:
  
  try { // Perform the test method. } catch (...) { if
  (GTEST_FLAG_GET(catch_exceptions)) // Report the exception as failure. else
  throw;  // Re-throws the original exception. }
  
  However, the purpose of this flag is to allow the program to drop into the
  debugger when the exception is thrown. On most platforms, once the control
  enters the catch block, the exception origin information is lost and the
  debugger will stop the program at the point of the re-throw in this function --
  instead of at the point of the original throw statement in the code under test.
  For this reason, we perform the check early, sacrificing the ability to affect
  Google Test's exception handling in the method where the exception is thrown.

- `[_libs/yaml-cpp/test/googletest-1.13.0/googletest/src/gtest.cc:2841]`
  that the object will not be null



# _libs/yaml-cpp/test/googletest-1.13.0/googletest/test

- `[_libs/yaml-cpp/test/googletest-1.13.0/googletest/test/googletest-death-test-test.cc:1211]`
  that this cannot verify whether the original handle is closed.

- `[_libs/yaml-cpp/test/googletest-1.13.0/googletest/test/googletest-death-test-test.cc:1216]`
  that this cannot verify whether the original handle is closed.

- `[_libs/yaml-cpp/test/googletest-1.13.0/googletest/test/gtest_unittest.cc:7312]`
  that a and b are distinct but compatible types.



# _libs/yaml-cpp/test/integration

- `[_libs/yaml-cpp/test/integration/handler_spec_test.cpp:1451]`
  I believe this is a bug in the YAML spec - it should be "# text\n\n"

- `[_libs/yaml-cpp/test/integration/handler_spec_test.cpp:1653]`
  I believe this is a bug in the YAML spec - it should be "value\n"

- `[_libs/yaml-cpp/test/integration/node_spec_test.cpp:987]`
  I believe this is a bug in the YAML spec - it should be "# text\n\n"

- `[_libs/yaml-cpp/test/integration/node_spec_test.cpp:1113]`
  I believe this is a bug in the YAML spec - it should be "value\n"



# _libs/yaml-cpp/util

- `[_libs/yaml-cpp/util/api.cpp:110]`
  in all likelihood, the memory for nodes "zero" and "one" is still allocated.
  How can it go away? Weak pointers?

