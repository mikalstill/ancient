// This file contains a big ugly lookup table of windows language identifiers
// The information was stolen off the web...
 
//Language, Language Id, Windows Identifier (LanguageID, SubLangID)
  typedef struct internal_winlang
{
  char *name;
   int id;
   char *lang;
   char *sublang;
 }
winlang;
winlang languages[] =
{
  
  {
  "Albanian", 0x041C, "LANG_ALBANIAN", "SUBLANG_DEFAULT"}
  , 
  {
  "Arabic (Saudi Arabia)", 0x0401, "LANG_ARABIC",
      "SUBLANG_ARABIC_SAUDI_ARABIA"}
  , 
  {
  "Arabic (Iraq)", 0x0801, "LANG_ARABIC", "SUBLANG_ARABIC_IRAQ"}
  , 
  {
  "Arabic (Egypt)", 0x0c01, "LANG_ARABIC", "SUBLANG_ARABIC_EGYPT"}
  , 
  {
  "Arabic (Libya)", 0x1001, "LANG_ARABIC", "SUBLANG_ARABIC_LIBYA"}
  , 
  {
  "Arabic (Algeria)", 0x1401, "LANG_ARABIC", "SUBLANG_ARABIC_ALGERIA"}
  , 
  {
  "Arabic (Morocco)", 0x1801, "LANG_ARABIC", "SUBLANG_ARABIC_MOROCCO"}
  , 
  {
  "Arabic (Tunisia)", 0x1c01, "LANG_ARABIC", "SUBLANG_ARABIC_TUNISIA"}
  , 
  {
  "Arabic (Oman)", 0x2001, "LANG_ARABIC", "SUBLANG_ARABIC_OMAN"}
  , 
  {
  "Arabic (Yemen)", 0x2401, "LANG_ARABIC", "SUBLANG_ARABIC_YEMEN"}
  , 
  {
  "Arabic (Syria)", 0x2801, "LANG_ARABIC", "SUBLANG_ARABIC_SYRIA"}
  , 
  {
  "Arabic (Jordan)", 0x2c01, "LANG_ARABIC", "SUBLANG_ARABIC_JORDAN"}
  , 
  {
  "Arabic (Lebanon)", 0x3001, "LANG_ARABIC", "SUBLANG_ARABIC_LEBANON"}
  , 
  {
  "Arabic (Kuwait)", 0x3401, "LANG_ARABIC", "SUBLANG_ARABIC_KUWAIT"}
  , 
  {
  "Arabic (U.A.E.)", 0x3801, "LANG_ARABIC", "SUBLANG_ARABIC_UAE"}
  , 
  {
  "Arabic (Bahrain)", 0x3c01, "LANG_ARABIC", "SUBLANG_ARABIC_BAHRAIN"}
  , 
  {
  "Arabic (Qatar", 0x4001, "LANG_ARABIC", "SUBLANG_ARABIC_QATAR"}
  , 
  {
  "Bahasa", 0x0421, "LANG_BAHASA", "SUBLANG_DEFAULT"}
  , 
  {
  "Bulgarian", 0x0402, "LANG_BULGARIAN", "SUBLANG_DEFAULT"}
  , 
  {
  "Catalan", 0x0403, "LANG_CATALAN", "SUBLANG_DEFAULT"}
  , 
  {
  "Chinese (Tranditional) / Chinese (Taiwan Region)", 0x0404,
      "LANG_CHINESE", "SUBLANG_CHINESE_TRADITIONAL"}
  , 
  {
  "Chinese (Hong Kong)", 0x0C04, "LANG_CHINESE", "SUBLANG_CHINESE_HONGKONG"}
  , 
  {
  "Chinese - PR China / Chinese (Simplified)", 0x0804, "LANG_CHINESE",
      "SUBLANG_CHINESE_SIMPLIFIED"}
  , 
  {
  "Chinese (Singapore)", 0x1004, "LANG_CHINESE", "SUBLANG_CHINESE_SINGAPORE"}
  , 
  {
  "Croatian", 0x041A, "LANG_CROATIAN", "SUBLANG_DEFAULT"}
  , 
  {
  "Czech", 0x0405, "LANG_CZECH", "SUBLANG_DEFAULT"}
  , 
  {
  "Danish", 0x0406, "LANG_DANISH", "SUBLANG_DEFAULT"}
  , 
  {
  "Dutch", 0x0413, "LANG_DUTCH", "SUBLANG_DUTCH"}
  , 
  {
  "Dutch (Belgian)", 0x0813, "LANG_DUTCH", "SUBLANG_DUTCH_BELGIAN"}
  , 
  {
  "English Australian", 0x0C09, "LANG_ENGLISH", "SUBLANG_ENGLISH_AUS"}
  , 
  {
  "English Canadian", 0x1009, "LANG_ENGLISH", "SUBLANG_ENGLISH_CAN"}
  , 
  {
  "English (New Zealand)", 0x1409, "LANG_ENGLISH", "SUBLANG_ENGLISH_NZ"}
  , 
  {
  "English (Irish)", 0x1809, "LANG_ENGLISH", "SUBLANG_ENGLISH_EIRE"}
  , 
  {
  "English UK", 0x0809, "LANG_ENGLISH", "SUBLANG_ENGLISH_UK"}
  , 
  {
  "English US", 0x0409, "LANG_ENGLISH", "SUBLANG_ENGLISH_US"}
  , 
  {
  "Estonian", 0x0425, "LANG_ESTONIAN", "SUBLANG_DEFAULT"}
  , 
  {
  "Finnish", 0x040B, "LANG_FINNISH", "SUBLANG_DEFAULT"}
  , 
  {
  "French", 0x040C, "LANG_FRENCH", "SUBLANG_FRENCH"}
  , 
  {
  "French (Belgian)", 0x080C, "LANG_FRENCH", "SUBLANG_FRENCH_BELGIAN"}
  , 
  {
  "French (Canadian)", 0x0C0C, "LANG_FRENCH", "SUBLANG_FRENCH_CANADIAN"}
  , 
  {
  "French (Swiss)", 0x100C, "LANG_FRENCH", "SUBLANG_FRENCH_SWISS"}
  , 
  {
  "German", 0x0407, "LANG_GERMAN", "SUBLANG_GERMAN"}
  , 
  {
  "German (Austria)", 0x0C07, "LANG_GERMAN", "SUBLANG_GERMAN_AUSTRIAN"}
  , 
  {
  "German (Luxembourg)", 0x1007, "LANG_GERMAN", "SUBLANG_GERMAN_LUXEMBOURG"}
  , 
  {
  "German (Liechtenstein)", 0x1407, "LANG_GERMAN",
      "SUBLANG_GERMAN_LIECHTENSTEIN"}
  , 
  {
  "German (Swiss)", 0x0807, "LANG_GERMAN", "SUBLANG_GERMAN_SWISS"}
  , 
  {
  "Greek", 0x0408, "LANG_GREEK", "SUBLANG_DEFAULT"}
  , 
  {
  "Hebrew", 0x040D, "LANG_HEBREW", "SUBLANG_DEFAULT"}
  , 
  {
  "Hungarian", 0x040E, "LANG_HUNGARIAN", "SUBLANG_DEFAULT"}
  , 
  {
  "Icelandic", 0x040F, "LANG_ICELANDIC", "SUBLANG_DEFAULT"}
  , 
  {
  "Italian", 0x0410, "LANG_ITALIAN", "SUBLANG_ITALIAN"}
  , 
  {
  "Italian (Swiss)", 0x0810, "LANG_ITALIAN", "SUBLANG_ITALIAN_SWISS"}
  , 
  {
  "Japanese", 0x0411, "LANG_JAPANESE", "SUBLANG_DEFAULT"}
  , 
  {
  "Korean", 0x0412, "LANG_KOREAN", "SUBLANG_DEFAULT"}
  , 
  {
  "Lithuanian", 0x0427, "LANG_LITHUANIAN", "SUBLANG_LITHUANIAN"}
  , 
  {
  "Lithuanian (Classic)", 0x0827, "LANG_LITHUANIAN",
      "SUBLANG_LITHUANIAN_CLASSIC"}
  , 
  {
  "Malay (Malaysia)", 0x043E, "LANG_MALAY", "SUBLANG_MALAY_MALAYSIA"}
  , 
  {
  "Malay (Brunei Darussalam)", 0x083E, "LANG_MALAY",
      "SUBLANG_MALAY_BRUNEI_DARUSSALAM"}
  , 
  {
  "Malayalam", 0x044C, "LANG_MALAYALAM", "SUBLANG_DEFAULT"}
  , 
  {
  "Nepali", 0x0461, "LANG_NEPALI", "SUBLANG_DEFAULT"}
  , 
  {
  "Nepali (India)", 0x0861, "LANG_NEPALI", "SUBLANG_NEPALI_INDIA"}
  , 
  {
  "Norwegian (Bokmal)", 0x0414, "LANG_NORWEGIAN", "SUBLANG_NORWEGIAN_BOKMAL"}
  , 
  {
  "Norwegian (Nynorsk)", 0x0814, "LANG_NORWEGIAN",
      "SUBLANG_NORWEGIAN_NYNORSK"}
  , 
  {
  "Polish", 0x0415, "LANG_POLISH", "SUBLANG_DEFAULT"}
  , 
  {
  "Portuguese", 0x0816, "LANG_PORTUGUESE", "SUBLANG_PORTUGUESE"}
  , 
  {
  "Portuguese (Brazilian)", 0x0416, "LANG_PORTUGUESE",
      "SUBLANG_PORTUGUESE_BRAZILIAN"}
  , 
  {
  "Rhaeto-Romanic", 0x0417, "LANG_RHAETO_ROMAN", "SUBLANG_DEFAULT"}
  , 
  {
  "Romanian", 0x0418, "LANG_ROMANIAN", "SUBLANG_DEFAULT"}
  , 
  {
  "Russian", 0x0419, "LANG_RUSSIAN", "SUBLANG_DEFAULT"}
  , 
  {
  "Serbian (Latin)", 0x081A, "LANG_SERBIAN", "SUBLANG_SERBIAN_LATIN"}
  , 
  {
  "Serbian (Cyrillic)", 0x0C1A, "LANG_SERBIAN", "SUBLANG_SERBIAN_CYRILLIC"}
  , 
  {
  "Slovak", 0x041B, "LANG_SLOVAK", "SUBLANG_DEFAULT"}
  , 
  {
  "Slovenian", 0x0424, "LANG_SLOVENIAN", "SUBLANG_DEFAULT"}
  , 
  {
  "Spanish (Castilian)", 0x040A, "LANG_SPANISH", "SUBLANG_SPANISH"}
  , 
  {
  "Spanish (Mexican)", 0x080A, "LANG_SPANISH,SUBLANG_SPANISH_MEXICAN"}
  , 
  {
  "Spanish (Modern)", 0x0C0A, "LANG_SPANISH", "SUBLANG_SPANISH_MODERN"}
  , 
  {
  "Swedish", 0x041D, "LANG_SWEDISH", "SUBLANG_DEFAULT"}
  , 
  {
  "Thai", 0x041e, "LANG_THAI", "SUBLANG_DEFAULT"}
  , 
  {
  "Turkish", 0x041F, "LANG_TURKISH", "SUBLANG_DEFAULT"}
  , 
  {
  "Vietnamese", 0x042A, "LANG_VIETNAMESE", "SUBLANG_DEFAULT"}
  , 
  {
  NULL, 0, NULL, NULL}
};


