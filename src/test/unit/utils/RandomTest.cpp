/* ---------------------------------------------------------------------
 * Numenta Platform for Intelligent Computing (NuPIC)
 * Copyright (C) 2013, Numenta, Inc.  Unless you have an agreement
 * with Numenta, Inc., for a separate license for this software code, the
 * following terms and conditions apply:
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero Public License version 3 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU Affero Public License for more details.
 *
 * You should have received a copy of the GNU Affero Public License
 * along with this program.  If not, see http://www.gnu.org/licenses.
 *
 * http://numenta.org/licenses/
 * ---------------------------------------------------------------------
 */

/**
 * @file
 */

#include <fstream>
#include <gtest/gtest.h>
#include <nupic/ntypes/MemStream.hpp>
#include <nupic/os/Env.hpp>
#include <nupic/utils/LoggingException.hpp>
#include <nupic/utils/Random.hpp>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>

using namespace nupic;

// Expected values with seed of 148
// Comparing against expected values ensures the same result
// on all platforms.
UInt32 expected[1000] = {
    33267067,   1308471064, 567525506,  744151466,  1514731226, 320263095,
    850223971,  272111712,  1447892216, 1051935310, 767613522,  1966421570,
    740590150,  238534644,  439141684,  1006390399, 683226375,  105163871,
    1148420320, 1897583466, 1364189210, 2056512848, 2012249758, 724656770,
    862951273,  739791114,  2018040533, 733864322,  181139176,  1764207967,
    1203036433, 214406243,  925195384,  1770561940, 958557709,  292442962,
    2090825035, 1808781680, 564554675,  1391233604, 713233342,  1332168197,
    1210171526, 1453823492, 1570702841, 1649313210, 312730244,  106445569,
    1754477081, 1461150564, 2004029035, 971182643,  1370179764, 1868795145,
    1695839414, 85647389,   461102611,  1566396299, 819511712,  642241788,
    1183120618, 2022548145, 856648031,  2108316002, 1645626437, 1815205741,
    253275317,  1588967825, 1476503773, 817829992,  832717781,  42253468,
    2514541,    2042889307, 1496076960, 1573217382, 1544718869, 1808807204,
    1679662951, 1151712303, 1122474120, 1536208338, 2122894946, 345170236,
    1257519835, 1671250712, 430817626,  1718622447, 1090163363, 1250329338,
    213380587,  125800334,  1125393835, 1070028618, 86632688,   623536625,
    737750711,  339908005,  65020802,   66770837,   1157737997, 897738583,
    109024305,  1160252538, 793144242,  1605101265, 585986273,  190379463,
    1266424822, 118165576,  1342091766, 241415294,  1654373915, 1317503065,
    586585531,  764410102,  841270129,  1017403157, 335548901,  1931433493,
    120248847,  548929488,  2057233827, 1245642682, 1618958107, 2143866515,
    1869179307, 209225170,  336290873,  1934200109, 275996007,  1494028870,
    684455044,  385020312,  506797761,  1477599286, 1990121578, 1092784034,
    1667978750, 1109062752, 1210949610, 862586868,  1350478046, 717839877,
    32606285,   1937063577, 1482249980, 873876415,  806983086,  1817798881,
    657826260,  927231933,  219244722,  567576439,  25390968,   1838202829,
    563959306,  1894570275, 2047427999, 900250179,  1681286737, 175940359,
    246795402,  218258133,  560960671,  753593163,  1695857420, 403598601,
    1846377197, 1216352522, 1512661353, 909843159,  2078939390, 715655752,
    1627683037, 2111545676, 505235681,  962449369,  837938443,  1312218768,
    632764602,  1495764703, 91967053,   852009324,  2063341142, 117358021,
    542728505,  479816800,  2011928297, 442672857,  1380066980, 1545731386,
    618613216,  1626862382, 1763989519, 1179573887, 232971897,  1312363291,
    1583172489, 2079349094, 381232165,  948350194,  841708605,  312687908,
    1664005946, 321907994,  276749936,  21757980,   1284357363, 1114688379,
    1333976748, 1917121966, 462969434,  1425943801, 621647642,  378826928,
    1543301823, 1164376148, 858643728,  1407746472, 1607049005, 91227060,
    805994210,  78178573,   1718089442, 422500081,  1257752460, 1951061339,
    1734863373, 693441301,  1882926785, 2116095538, 1641791496, 577151743,
    281299798,  1158313794, 899059737,  558049734,  1180071774, 35933453,
    1672738113, 366564874,  1953055419, 2135707547, 1792508676, 427219413,
    367050827,  1188326851, 1591595561, 1225694556, 448589675,  1051160918,
    1316921616, 1254583885, 1129339491, 887527411,  1677083966, 239608304,
    691105102,  1264463691, 933049605,  426548240,  1233075582, 427357453,
    1003699983, 1514375380, 1585671248, 1902759720, 2072425115, 618259374,
    1938693173, 1597679580, 984824249,  1744264944, 1585903480, 629849277,
    24000710,   1952954307, 1818176128, 1615596271, 1031165215, 119282155,
    519273542,  200603184,  1373866040, 1648613033, 1088130595, 903466358,
    1888221337, 1779235697, 20446402,   673787295,  58300289,   1253521984,
    1101144748, 1062000272, 620413716,  539332348,  817276345,  545355183,
    1157591723, 608485870,  2143034764, 2142415972, 205267167,  1581454596,
    624781601,  229267877,  1386925255, 295474081,  1844864148, 270606823,
    414756236,  216654042,  471210007,  1788622276, 1865267076, 1559340602,
    544604986,  1606004765, 1191092651, 565051388,  132308412,  1249392941,
    1818573372, 1233453161, 163909565,  291503441,  1772785509, 981185910,
    836858624,  782893584,  1589671781, 832409740,  777825908,  1794938948,
    266380688,  1402607509, 2024206825, 1653305944, 1698081590, 1721587325,
    1923912767, 2112837826, 1938241368, 247639126,  1753976454, 1656024796,
    1806979728, 151097793,  1114545913, 850588731,  716149181,  1246854326,
    2099981672, 387238906,  332823839,  116407590,  678742347,  2105609348,
    1097593500, 1515600971, 741019285,  539781633,  200527064,  1518845193,
    187236933,  466907752,  773969055,  63960110,   2120213696, 324566997,
    1785547436, 1896642815, 289921176,  1576305156, 2144281941, 2043897630,
    1084846304, 1803778021, 47511775,   51908569,   506883105,  763660957,
    1298762895, 459381129,  1150899863, 1631586734, 575788719,  1829642210,
    1589712435, 1673382220, 1197759533, 183248072,  65680205,   1398286597,
    1702093265, 252917139,  1865194350, 328578672,  316877249,  1837924398,
    653145670,  2102424685, 1587083566, 943066846,  1531246193, 1583881859,
    839480828,  468608849,  1240176233, 886992604,  520517419,  1747059338,
    1650653561, 1819280314, 58956819,   654069776,  1303383401, 634745539,
    336228338,  745612188,  160644111,  1533987871, 928860260,  226324316,
    784790821,  483469877,  479241455,  502501523,  812048550,  796118705,
    192942273,  1465194220, 751059742,  1780025839, 260777418,  134822288,
    1216424051, 1100258246, 603431137,  309116636,  1987250850, 1123948556,
    2056175974, 1490420763, 795745223,  2115132793, 2144490539, 2099128624,
    602394684,  333235229,  697257164,  763038795,  1867223101, 1626117424,
    989363112,  504530274,  2109587301, 1468604567, 1007031797, 774152203,
    117239624,  1199974070, 91862775,   868299367,  832516262,  352640193,
    1003121655, 2048940313, 1452898440, 1606552792, 210573301,  1292665642,
    583017701,  119265627,  635602758,  1378762924, 86914772,   632609649,
    1330407900, 689309457,  965844879,  2027665064, 1452348252, 685584332,
    1506298840, 294227716,  1190114606, 1468402493, 1762832284, 49662755,
    95071049,   1880071908, 1249636825, 186933824,  600887627,  2082153087,
    539574018,  1604009282, 1983609752, 1992472458, 1063078427, 46699405,
    1137654452, 1646096128, 165965032,  1773257210, 877375404,  252879805,
    258383212,  60299656,   942189262,  1224228091, 2087964720, 247053866,
    1909812423, 1446779912, 541281583,  952443381,  767698757,  156630219,
    1002106136, 862769806,  2036702127, 104259313,  1049703631, 490106107,
    38928753,   1589277649, 2094115389, 2022538505, 1434266459, 1009710168,
    2069237911, 424437263,  508322648,  87719295,   50210826,   1385698052,
    340599100,  308594038,  1445997708, 1282788362, 1532822129, 1386478780,
    1529842229, 1295150904, 685775044,  2071123812, 100110637,  1453473802,
    80270383,   1102216773, 168759960,  2116972510, 1206476086, 1218463591,
    459594969,  1245404839, 660257592,  406226711,  1120459697, 2094524051,
    1415936879, 1042213960, 371477667,  1924259528, 1129933255, 421688493,
    1162473932, 1470532356, 730282531,  460987993,  605837070,  115621012,
    1847466773, 2135679299, 1410771916, 385758170,  2059319463, 1510882553,
    1839231972, 2139589846, 465615678,  2007991932, 2109078709, 1672091764,
    1078971876, 421190030,  770012956,  1739229468, 827416741,  1890472653,
    1686269872, 95869973,   785202965,  2057747539, 2020129501, 1915136220,
    331952384,  1035119785, 1238184928, 1062234915, 1496107778, 1844021999,
    1177855927, 1196090904, 1832217650, 441144195,  1581849074, 1744053466,
    1952026748, 1273597398, 1736159664, 270158778,  1134105682, 1697754725,
    1942250542, 65593910,   2118944756, 564779850,  1804823379, 798877849,
    307768855,  1343609603, 894747822,  1092971820, 1253873494, 767393675,
    860624393,  1585825878, 1802513461, 2098809321, 500577145,  1151137591,
    1795347672, 1678433072, 199744847,  1480081675, 2119577267, 1781593921,
    1076651493, 1924120367, 907707671,  665327509,  46795497,   2041813354,
    215598587,  1989046039, 2107407264, 187059695,  406342242,  1764746995,
    985937544,  714111097,  960872950,  1880685367, 1807082918, 67262796,
    500595394,  520223663,  1653088674, 155625207,  471549336,  6182171,
    1306762799, 119413361,  1684615243, 1506507646, 1599495036, 1656708862,
    1140617920, 528662881,  1433345581, 2048325591, 1193990390, 1480141078,
    1942655297, 1409588977, 1321703470, 1902578914, 1596648672, 1728045712,
    1519842261, 435102569,  294673161,  333231564,  168304288,  2101756079,
    400494360,  668899682,  474496094,  2053583035, 824524890,  946045431,
    2059765206, 2131287689, 1065458792, 1596896802, 1490311687, 517470180,
    1106122016, 483445959,  1046133061, 391983950,  384287903,  92639803,
    1872125028, 179459552,  1502228781, 1046344850, 2082038466, 951393805,
    626906914,  1454397080, 1386496374, 921580076,  1787628644, 1554800662,
    875852507,  40639356,   76216697,   1350348602, 2094222391, 900741587,
    148910385,  2006503950, 884545628,  1214369177, 1455917104, 227373667,
    1731839357, 414555472,  710819627,  630488770,  806539422,  1095107530,
    723128573,  531180803,  1274567082, 77873706,   1577525653, 1209121901,
    1029267512, 56948920,   516035333,  268280238,  978528996,  156180329,
    1823080901, 1854381503, 196819685,  1899297598, 1057246457, 143558429,
    652555537,  1206156842, 2578731,    1537101165, 273042371,  1458495835,
    1764474832, 2004881728, 1873051307, 327810811,  487886850,  532107082,
    1422918341, 1211015424, 1063287885, 550001776,  1288889130, 493329890,
    1759123677, 170672994,  550278810,  127675362,  438953233,  1528807806,
    283855691,  114550486,  1235705662, 480675376,  2013848084, 145468471,
    624233805,  518919973,  1351625314, 626812536,  2056021138, 1624667685,
    2085308371, 1673012322, 1482065766, 1810876031, 2000823134, 1969952616,
    195499465,  1276257827, 1033484392, 1258787350, 1826259603, 174889875,
    1752117240, 1437899632, 345562869,  154912403,  1565574994, 784516102,
    1683720209, 1849430685, 899066588,  771942223,  182622414,  765431024,
    917410695,  806856219,  1284350997, 121552361,  1433668756, 1192888487,
    1746220046, 1371493479, 718417162,  1080802164, 1034885862, 571756648,
    903271133,  1230385327, 1848014475, 1936755525, 341689029,  1526790431,
    2111645400, 2093806270, 817206415,  309724622,  101235025,  235297762,
    1094240724, 1784955234, 2084728447, 1993307313, 409413810,  119867213,
    611254689,  1326824505, 926723433,  1895605687, 1448376866, 212908541,
    941010526,  1047113264, 1584402020, 1659427688, 2127915429, 471804235,
    83700688,   883702914,  1702189562, 1931715164, 672974791,  2043878592,
    1311021947, 637136544,  1990201214, 2128228362, 946861166,  2091436239,
    216042476,  2041101890, 1728907825, 153287276,  1886925555, 2138321635,
    273154489,  350696597,  1317662492, 1199877922, 98818636,   618555710,
    1412786463, 1039829162, 1665668975, 849704836,  551773203,  1646100756,
    1321509071, 635473891,  382320022,  876214985,  419705407,  1055294813,
    772609929,  1730727354, 1692431357, 615327495,  1711472069, 491808875,
    559280086,  1927514545, 385427118,  140704264,  2080801821, 124869025,
    131542251,  206472663,  475565622,  1449204744, 1406350585, 574384258,
    2067760454, 671653401,  1614213421, 1585945781, 1521358237, 18502976,
    1084562889, 695383660,  653976867,  1466882911, 1571598645, 1073682275,
    374694077,  196724927,  656925981,  2067125434, 812052422,  220914402,
    411450662,  1371332509, 945300,     796877780,  1512036773, 2081747121,
    921746805,  1643579024, 140736136,  1397312428, 945300120,  1547086722,
    1971696686, 865576927,  71256475,   1438426459, 304039060,  1592614712,
    1456929435, 1388601950, 140514724,  2110906303, 708001213,  1712113369,
    1037104930, 1082695290, 1908838296, 1694030911, 1002337077, 573407071,
    1914945314, 1413787739, 1944739580, 1915890614, 63181871,   1309292705,
    1850154087, 984928676,  805388081,  1990890224, 234757456,  1750688202,
    1390493298, 58970495,   468781481,  1461749773, 1497396954, 772820541,
    906880837,  806842742,  13938843,   1047395561, 770265397,  721940057,
    612025282,  1807370327, 1804635347, 373379931,  1353917590, 659488776,
    946787002,  1121379256, 2073276515, 744042934,  889786222,  2136458386,
    2053335639, 592456662,  973903415,  711240072};

TEST(RandomTest, Seeding) {
  // make sure the global instance is seeded from time()
  // in the test situation, we can be sure we were seeded less than 100000
  // seconds ago make sure random number system is initialized by creating a
  // random object. Use the object to make sure the compiler doesn't complain
  // about an unused variable
  Random r;
  UInt64 x = r.getUInt64();
  ASSERT_TRUE(x != 0);

  // test getSeed
  {
    Random r(98765);
    ASSERT_EQ(98765U, r.getSeed());
  }
}

TEST(RandomTest, CopyConstructor) {
  // test copy constructor.
  Random r1(289436);
  for (int i = 0; i < 100; i++)
    r1.getUInt32();
  Random r2(r1); //copy

  UInt32 v1, v2;
  for (int i = 0; i < 100; i++) {
    v1 = r1.getUInt32();
    v2 = r2.getUInt32();
    ASSERT_EQ(v1, v2) << "copy constructor";
  }
}

TEST(RandomTest, OperatorEquals) {
  // test operator=
  Random r1(289436);
  for (int i = 0; i < 100; i++)
    r1.getUInt32();
  Random r2(86726008);
  for (int i = 0; i < 100; i++)
    r2.getUInt32();

  r2 = r1;
  UInt32 v1, v2;
  for (int i = 0; i < 100; i++) {
    v1 = r1.getUInt32();
    v2 = r2.getUInt32();
    ASSERT_EQ(v1, v2) << "operator=";
  }
}

TEST(RandomTest, SerializationDeserialization) {
  // test serialization/deserialization
  Random r1(862973);
  for (int i = 0; i < 100; i++)
    r1.getUInt32();

  // serialize
  OMemStream ostream;
  ostream << r1;

  // print out serialization for debugging
  std::string x(ostream.str(), ostream.pcount());
  NTA_INFO << "random serialize string: '" << x << "'";
  // Serialization should be deterministic and platform independent
  const std::string expectedString = "random-v2 862973 18278996500901106032 15671709857590179817 8649143637613862617 14264429998139648216 7771264048295431516 12226518405605705467 9772830111674237957 2323109042872219490 1972267838377633676 4143805970759863142 4624774728382164762 12935978031408885367 6972487223327110659 15846938151234461964 11219293672540080171 14696237751206470269 6237251308076371808 18303454838236991452 14830048466723621352 13323371025612656614 1037858307069662128 16624817170464937049 8537662857527322758 7385864444757712918 17956570412639773486 4855053662616968764 3655495107089919144 6157688543085567507 13640860710510529999 13745496315366170980 18190758010941307027 10888058020215811933 15457426345680933739 13222266253960919258 14000531053380469081 6445962488720243440 6759827609280687257 9685298126530559754 11839328416613730208 6900349664257781977 10412050736607118977 9124061408639164216 1823310794874452157 7096362482785184925 1021293691771630067 13476089715767250826 15728463420046675209 13565809498496819777 9240971383622733872 11786532890618023851 4466092817536822683 12138060308925662376 9779492982347927329 2296651319953831363 6486918918849116870 6589002633859405289 14867336130489681166 1192548725840495690 13141752414421483991 11134645008815067354 14583602513622966895 15960947115193958410 18353069494231827305 3351756852925883741 13485064114979771020 6339190459929307581 6092493536042535648 12775490997039948508 16938918407794598942 12425986851697247133 10077604765558312825 3841293312798158517 18226409647793166463 15053767120575981552 10123661549063671344 5451779945311442643 8601932045316207235 12925933527899203565 521567204405744150 11238568895268365923 10963397621656504303 1961818930936664622 9627734219248346861 4937607601245600553 13901192365207114619 3386902976004297107 2673374161914925356 5311167795143569620 6568893266240336821 2655161243531266295 5612167271838000560 13735243134516592904 4533745535730853329 5448929406991236112 14788774535855437970 14077606640909812000 5912648644150041866 7397160262012255201 12203995080258673397 7086710291991524617 18068923821240152691 11617645084918574301 3329015836024472831 4750448713647126429 12672508906530021165 13559104084492292096 1577478886855312205 5445593580097745559 5241014873998135932 6809582639954126746 12146628204439728391 6347410113983010931 4484239761900545258 2344706167516049994 10633130995961744046 2481454355401363631 1027371674501805941 11042740960021817330 2180515875846820979 2686316371058090993 6460108757191978129 11428255416650003160 1032255042732803172 9134069054302974144 10207956204260944197 11652197594967249178 16314103346691746127 12219728684028921059 2659434015512574443 7962968320374692538 10454351847394346376 9575019991700933390 4163964851284949654 6837582126540524893 16287742693491758850 5698534069126207329 8674356181128846427 6041854520491811047 14922093730153036266 14651125709342308795 865270013954178319 2464469368795060466 9515299453954011486 9904175940778356811 8684237949091436560 14641265258263876797 11194007137179012099 14607180489197747573 8716538491530480493 1995915804673391617 5797631934988564357 8259972137289137075 2787555451068468396 1312786590030608818 2628654432258593333 6020174472748330373 3924992758398899568 13660892272916950526 14592343489533950058 11629562707738206359 877184951808404161 11984873935423627025 12485568272296960015 17388610391211746396 14299035841800892309 9487901976514028950 9558989529141027192 10421458983121991348 14578987201341601905 2697551916353073944 2093006244159606540 3415819640709931227 15574273391924406448 1652897107963863536 159561568570104266 5884301525680715903 10185106882991485009 10785392918505111223 1174927153603916917 13482484179862035845 12406172019294686654 38969327760228580 4764185743914090431 651696507396561705 2077852583962748328 4785438127163514779 13102841800288845676 4889908843365015342 6669624380153822450 25974141135159446 5888513430134971856 7608760962656987312 17687280854384181456 16364916102675559332 9566134384197176110 15779916726996258782 9298908011080331086 3745889474208650401 11548862675781046300 14002498090823963862 14087913786702688017 15453582353246649517 12329476228190886563 3956676840709129590 9571200147725827891 17217472948481920618 9881170942923176486 10401220325915702660 8154957476331859609 12242491706491238678 4686095853942588746 2171661981660433114 8919955030613661062 17891525274857953346 12576832254897120856 9783219616832828757 3608296164371740644 2411003170075841417 17865025775570954703 2896991860734409921 5038941393951403874 4401839680434628288 11990859466063097394 11011664522716686867 729091238439240529 405464414201218881 2105250740906867538 14316580569859555091 4530109736278223294 7735650894519687432 2159639240650283689 11926873877772625117 12984655867977120392 5625588315509063670 9834755213541076480 2648172552609894741 10779660933474245039 68694957598655385 17536418038029877729 15025345101696676842 4826191464454688840 10385672626836887067 18309707544522964892 16688836695277258888 15668185175402533399 2135033745817588317 3620472290618138769 7844983645648051795 16579044331202905803 4429517479599118103 16831442372416762333 4890548298637904702 6545819644898104970 12201403701441111754 13538794021453860549 2779877820474353940 10094184035498551152 7856904176185743941 13718516137743924040 7006617972029757077 1440336980799782958 11318739778840911269 8629079412510769827 9848523213047448419 16054080765990091528 4417862554603522155 11011963754153905534 5466764592503369912 9773811298627394614 6176149347085553889 7480422445734772014 6367834212987818111 2755765288872895450 15037281496215311085 7247057054276135957 13365840541909761558 15132626710360857720 7340507774942928767 1592913228766078908 13606471584807705966 10250305077644017883 4423427859991722522 17723649430940885846 16583080531212839461 84625348770499352 16807952791759356559 6617722655640759049 9135334039821059708 16505768376549877469 17874867669214007061 16699545654545788279 9532320386480528388 2607353342030129577 11300198053561790958 10111831878880614237 1769233306485334138 6263025804159607663 18005808381583079203 10242005106370175322 5138829258521552303 4535725279156783705 15998214446053587665 18329741359276666964 13940851456877050262 5228185289733632635 10016003782550345445 5333604436908956668 12774602902768122642 9191004509283870122 11175903787506028644 3116226344295902832 3491691098733094205 100 0 4294967295 0 18446744073709551615 0.00000000000000000e+00 1.00000000000000000e+00 endrandom-v2 "; 
  ASSERT_EQ(expectedString, x);

  // deserialize into r2
  std::string s(ostream.str(), ostream.pcount());
  std::stringstream ss(s);
  Random r2;
  ss >> r2;

  // r1 and r2 should be identical
  ASSERT_EQ(r1.getSeed(), r2.getSeed());

  UInt32 v1, v2;
  for (int i = 0; i < 100; i++) {
    v1 = r1.getUInt32();
    v2 = r2.getUInt32();
    ASSERT_EQ(v1, v2) << "serialization";
  }
}


TEST(RandomTest, ReturnInCorrectRange) {
  // make sure that we are returning values in the correct range
  // @todo perform statistical tests
  Random r;
  UInt32 seed = r.getSeed();
  ASSERT_TRUE(seed != 0) << "seed not zero";
  int i;
  UInt32 max32 = 10000000;
  UInt64 max64 = (UInt64)max32 * (UInt64)max32;
  for (i = 0; i < 200; i++) {
    UInt32 i32 = r.getUInt32(max32);
    ASSERT_TRUE(i32 < max32) << "UInt32";
    UInt64 i64 = r.getUInt64(max64);
    ASSERT_TRUE(i64 < max64) << "UInt64";
    Real64 r64 = r.getReal64();
    ASSERT_TRUE(r64 >= 0.0 && r64 < 1.0) << "Real64";
  }
}


TEST(RandomTest, getUInt64) {
  // tests for getUInt64
  Random r1(1);
  ASSERT_EQ(2469588189546311528u, r1.getUInt64())
      << "check getUInt64, seed 1, first call";
  ASSERT_EQ(2516265689700432462u, r1.getUInt64())
      << "check getUInt64, seed 1, second call";

  Random r2(2);
  ASSERT_EQ(16668552215174154828u, r2.getUInt64())
      << "check getUInt64, seed 2, first call";
  EXPECT_EQ(15684088468973760345u, r2.getUInt64())
      << "check getUInt64, seed 2, second call";

  Random r3(7464235991977222558);
  EXPECT_EQ(8035066300482877360u, r3.getUInt64())
      << "check getUInt64, big seed, first call";
  EXPECT_EQ(623784303608610892u, r3.getUInt64())
      << "check getUInt64, big seed, second call";
}


TEST(RandomTest, getReal64) {
  // tests for getReal64
  Random r1(1);
  EXPECT_DOUBLE_EQ(0.13387664401253274, r1.getReal64());
  EXPECT_DOUBLE_EQ(0.13640703636619725, r1.getReal64());

  Random r2(2);
  EXPECT_DOUBLE_EQ(0.90360402619399427, r2.getReal64());
  EXPECT_DOUBLE_EQ(0.85023613957581001, r2.getReal64());

  Random r3(7464235991977222558);
  EXPECT_DOUBLE_EQ(0.43558181695243003, r3.getReal64());
  EXPECT_DOUBLE_EQ(0.033815414856740673, r3.getReal64());
}


TEST(RandomTest, Sampling) {
  // tests for sampling

  const UInt32 population[] = {1, 2, 3, 4};
  Random r(1);

  {
    // choose some elements
    UInt32 choices[2];
    r.sample(population, 4, choices, 2);
    ASSERT_EQ(3, choices[0]) << "check sample 0";
    ASSERT_EQ(4, choices[1]) << "check sample 1";
  }

  {
    // choose all elements
    UInt32 choices[4];
    r.sample(population, 4, choices, 4);
    ASSERT_EQ(4, choices[0]) << "check sample 0";
    EXPECT_EQ(1, choices[1]) << "check sample 1";
    EXPECT_EQ(2, choices[2]) << "check sample 2";
    EXPECT_EQ(3, choices[3]) << "check sample 3";
  }

  //check population list remained unmodified
  ASSERT_EQ(1, population[0]) << "check sample p 0";
  ASSERT_EQ(2, population[1]) << "check sample p 1";
  ASSERT_EQ(3, population[2]) << "check sample p 2";
  ASSERT_EQ(4, population[3]) << "check sample p 3";

  {
    // nChoices > nPopulation
    UInt32 choices[5];
    bool caught = false;
    try {
      r.sample(population, 4, choices, 5);
    } catch (LoggingException &exc) {
      caught = true;
    }
    ASSERT_TRUE(caught) << "checking for exception from population too small";
  }
}


TEST(RandomTest, Shuffling) {
  // tests for shuffling
  Random r(1);
  UInt32 arr[] = {1, 2, 3, 4};
  const UInt32 exp[] = {3, 4, 2, 1};

  r.shuffle(std::begin(arr), std::end(arr));

  EXPECT_EQ(exp[0], arr[0]) << "check shuffle 0";
  EXPECT_EQ(exp[1], arr[1]) << "check shuffle 1";
  EXPECT_EQ(exp[2], arr[2]) << "check shuffle 2";
  EXPECT_EQ(exp[3], arr[3]) << "check shuffle 3";
}


/**
 * Test operator '=='
 */
TEST(RandomTest, testEqualsOperator) {
  Random r1(42), r2(42), r3(3);
  ASSERT_EQ(r1, r2);
  ASSERT_TRUE(r1 != r3);
  ASSERT_TRUE(r2 != r3);

  UInt32 v1, v2;
  v1 = r1.getUInt32();
  ASSERT_TRUE(r1 != r2);
  v2 = r2.getUInt32();
  ASSERT_TRUE(r1 == r2);
  ASSERT_EQ(v1, v2);
}


TEST(RandomTest, testGetUIntSpeed) {
 Random r1(42);
 UInt32 rnd;
 const int RUNS = 10000000;
 for(int i=0; i<RUNS; i++) {
   rnd = r1.getUInt32(10000); //get random int [0..1M)
   NTA_CHECK(rnd < 1000001) << "Should never happen, just here so compiler won't optimize out this loop";
 }
}
