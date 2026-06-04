#include "lvgl.h"
#include <stdio.h>
#include "gui_guider.h"
#include "events_init.h"
#include "widgets_init.h"
#include "custom.h"
#include "../../../../../../application/rt-thread/helloworld/main.h"

uint8_t city_cnt[34] = {2, 7, 11, 11, 12, 14, 9, 13, 7, 13, 11, 16, 9, 11, 17, 18, 17, 14, 21, 14, 19, 8, 21, 9, 16, 7, 11, 14, 8, 5, 23, 3, 1, 1};

char *city_province = "北京\n天津\n河北\n山西\n内蒙古\n辽宁\n吉林\n黑龙江\n上海\n江苏\n浙江\n安徽\n福建\n江西\n\
山东\n河南\n湖北\n湖南\n广东\n广西\n海南\n重庆\n四川\n贵州\n云南\n西藏\n陕西\n甘肃\n青海\n宁夏\n新疆\n台湾\n香港\n澳门";

// 保持原有数组结构，仅将中文替换为标准英文翻译
char *city_province_en = "Beijing\nTianjin\nHebei\nShanxi\nInner Mongolia\nLiaoning\nJilin\nHeilongjiang\nShanghai\nJiangsu\nZhejiang\nAnhui\nFujian\nJiangxi\n\
Shandong\nHenan\nHubei\nHunan\nGuangdong\nGuangxi\nHainan\nChongqing\nSichuan\nGuizhou\nYunnan\nTibet\nShaanxi\nGansu\nQinghai\nNingxia\nXinjiang\nTaiwan\nHong Kong\nMacao";

city_t my_city_2[34] = {
    "北京\n西城",                                                                                                                                                                     // 2
    "天津\n和平\n河东\n河西\n南开\n河北\n红桥",                                                                                                                                       // 7
    "石家庄\n唐山\n秦皇岛\n邯郸\n邢台\n保定\n张家口\n承德\n沧州\n廊坊\n衡水",                                                                                                         // 11
    "太原\n大同\n阳泉\n长治\n晋城\n朔州\n晋中\n运城\n忻州\n临汾\n吕梁",                                                                                                               // 11
    "呼和浩特\n包头\n乌海\n赤峰\n通辽\n鄂尔多斯\n呼伦贝尔\n巴彦淖尔\n乌兰察布\n兴安盟\n锡林郭勒\n阿拉善盟",                                                                           // 12
    "沈阳\n大连\n鞍山\n抚顺\n本溪\n丹东\n锦州\n营口\n阜新\n辽阳\n盘锦\n铁岭\n朝阳\n葫芦岛",                                                                                           // 14
    "长春\n吉林\n四平\n辽源\n通化\n白山\n松原\n白城\n延边",                                                                                                                           // 9
    "哈尔滨\n齐齐哈尔\n鸡西\n鹤岗\n双鸭山\n大庆\n伊春\n佳木斯\n七台河\n牡丹江\n黑河\n绥化\n大兴安岭",                                                                                 // 13
    "上海\n黄浦\n长宁\n静安\n普陀\n虹口\n杨浦\n",                                                                                                                                     // 7
    "南京\n无锡\n徐州\n常州\n苏州\n南通\n连云港\n淮安\n盐城\n扬州\n镇江\n泰州\n宿迁",                                                                                                 // 13
    "杭州\n宁波\n温州\n嘉兴\n湖州\n绍兴\n金华\n衢州\n舟山\n台州\n丽水",                                                                                                               // 11
    "合肥\n芜湖\n蚌埠\n淮南\n马鞍山\n淮北\n铜陵\n安庆\n黄山\n滁州\n阜阳\n宿州\n六安\n亳州\n池州\n宣城",                                                                               // 16
    "福州\n厦门\n莆田\n三明\n泉州\n漳州\n南平\n龙岩\n宁德",                                                                                                                           // 9
    "南昌\n景德镇\n萍乡\n九江\n新余\n鹰潭\n赣州\n吉安\n宜春\n抚州\n上饶",                                                                                                             // 11
    "济南\n莱芜\n青岛\n淄博\n枣庄\n东营\n烟台\n潍坊\n济宁\n泰安\n威海\n日照\n临沂\n德州\n聊城\n滨州\n菏泽",                                                                           // 17
    "郑州\n开封\n洛阳\n平顶山\n安阳\n鹤壁\n新乡\n焦作\n濮阳\n许昌\n漯河\n三门峡\n南阳\n商丘\n信阳\n周口\n驻马店\n济源",                                                               // 18
    "武汉\n黄石\n十堰\n宜昌\n襄阳\n鄂州\n荆门\n孝感\n荆州\n黄冈\n咸宁\n随州\n恩施\n仙桃\n潜江\n天门\n神农架",                                                                         // 17
    "长沙\n株洲\n湘潭\n衡阳\n邵阳\n岳阳\n常德\n张家界\n益阳\n郴州\n永州\n怀化\n娄底\n湘西",                                                                                           // 14
    "广州\n韶关\n深圳\n珠海\n汕头\n佛山\n江门\n湛江\n茂名\n肇庆\n惠州\n梅州\n汕尾\n河源\n阳江\n清远\n东莞\n中山\n潮州\n揭阳\n云浮",                                                   // 21
    "南宁\n柳州\n桂林\n梧州\n北海\n防城港\n钦州\n贵港\n玉林\n百色\n贺州\n河池\n来宾\n崇左",                                                                                           // 14
    "海口\n三亚\n三沙\n儋州\n五指山\n琼海\n文昌\n万宁\n东方\n定安\n屯昌\n澄迈\n临高\n白沙\n昌江\n乐东\n陵水\n保亭\n琼中",                                                             // 19
    "重庆\n渝中\n大渡口\n江北\n沙坪坝\n九龙坡\n南岸\n开州\n",                                                                                                                         // 8
    "成都\n自贡\n攀枝花\n泸州\n德阳\n绵阳\n广元\n遂宁\n内江\n乐山\n南充\n眉山\n宜宾\n广安\n达州\n雅安\n巴中\n资阳\n阿坝\n甘孜\n凉山",                                                 // 21
    "贵阳\n六盘水\n遵义\n安顺\n毕节\n铜仁\n黔西南\n黔东南\n黔南",                                                                                                                     // 9
    "昆明\n曲靖\n玉溪\n保山\n昭通\n丽江\n普洱\n临沧\n楚雄\n红河\n文山\n西双版纳\n大理\n德宏\n怒江\n迪庆",                                                                             // 16
    "拉萨\n日喀则\n昌都\n林芝\n山南\n那曲\n阿里",                                                                                                                                     // 7
    "西安\n铜川\n宝鸡\n杨凌\n咸阳\n渭南\n延安\n汉中\n榆林\n安康\n商洛",                                                                                                               // 11
    "兰州\n嘉峪关\n金昌\n白银\n天水\n武威\n张掖\n平凉\n酒泉\n庆阳\n定西\n陇南\n临夏\n甘南",                                                                                           // 14
    "西宁\n海东\n海北\n黄南\n海南\n果洛\n玉树\n海西",                                                                                                                                 // 8
    "银川\n石嘴山\n吴忠\n固原\n中卫\n",                                                                                                                                               // 5
    "乌鲁木齐\n克拉玛依\n吐鲁番\n哈密\n昌吉\n博尔塔拉\n巴音郭楞\n阿克苏\n克州\n喀什\n和田\n伊犁\n塔城\n阿勒泰\n石河子\n阿拉尔\n图木舒克\n五家渠\n北屯\n铁门关\n双河\n可克达拉\n昆玉", // 23
    "台北\n高雄\n台中",
    "香港",
    "澳门"};
// 保持原有数组结构，中文替换为标准英文翻译
city_t my_city_2_en[34] = {
    "Beijing\nXicheng",                                                                                                                                                                     // 2
    "Tianjin\nHeping\nHedong\nHexi\nNankai\nHebei\nHongqiao",                                                                                                                               // 7
    "Shijiazhuang\nTangshan\nQinhuangdao\nHandan\nXingtai\nBaoding\nZhangjiakou\nChengde\nCangzhou\nLangfang\nHengshui",                                                                     // 11
    "Taiyuan\nDatong\nYangquan\nChangzhi\nJincheng\nShuozhou\nJinzhong\nYuncheng\nXinzhou\nLinfen\nLvliang",                                                                                 // 11
    "Hohhot\nBaotou\nWuhai\nChifeng\nTongliao\nOrdos\nHulunbuir\nBayannur\nUlanqab\nHinggan League\nXilingol League\nAlxa League",                                                           // 12
    "Shenyang\nDalian\nAnshan\nFushun\nBenxi\nDandong\nJinzhou\nYingkou\nFuxin\nLiaoyang\nPanjin\nTieling\nChaoyang\nHuludao",                                                               // 14
    "Changchun\nJilin\nSiping\nLiaoyuan\nTonghua\nBaishan\nSongyuan\nBaicheng\nYanbian",                                                                                                     // 9
    "Harbin\nQiqihar\nJixi\nHegang\nShuangyashan\nDaqing\nYichun\nJiamusi\nQitaihe\nMudanjiang\nHeihe\nSuihua\nDaxing'anling",                                                               // 13
    "Shanghai\nHuangpu\nChangning\nJing'an\nPutuo\nHongkou\nYangpu\n",                                                                                                                     // 7
    "Nanjing\nWuxi\nXuzhou\nChangzhou\nSuzhou\nNantong\nLianyungang\nHuai'an\nYancheng\nYangzhou\nZhenjiang\nTaizhou\nSuqian",                                                               // 13
    "Hangzhou\nNingbo\nWenzhou\nJiaxing\nHuzhou\nShaoxing\nJinhua\nQuzhou\nZhoushan\nTaizhou\nLishui",                                                                                     // 11
    "Hefei\nWuhu\nBengbu\nHuainan\nMa'anshan\nHuaibei\nTongling\nAnqing\nHuangshan\nChuzhou\nFuyang\nSuzhou\nLu'an\nBozhou\nChizhou\nXuancheng",                                             // 16
    "Fuzhou\nXiamen\nPutian\nSanming\nQuanzhou\nZhangzhou\nNanping\nLongyan\nNingde",                                                                                                     // 9
    "Nanchang\nJingdezhen\nPingxiang\nJiujiang\nXinyu\nYingtan\nGanzhou\nJi'an\nYichun\nFuzhou\nShangrao",                                                                                 // 11
    "Jinan\nLaiwu\nQingdao\nZibo\nZaozhuang\nDongying\nYantai\nWeifang\nJining\nTai'an\nWeihai\nRizhao\nLinyi\nDezhou\nLiaocheng\nBinzhou\nHeze",                                           // 17
    "Zhengzhou\nKaifeng\nLuoyang\nPingdingshan\nAnyang\nHebi\nXinxiang\nJiaozuo\nPuyang\nXuchang\nLuohe\nSanmenxia\nNanyang\nShangqiu\nXinyang\nZhoukou\nZhumadian\nJiyuan",                   // 18
    "Wuhan\nHuangshi\nShiyan\nYichang\nXiangyang\nEzhou\nJingmen\nXiaogan\nJingzhou\nHuanggang\nXianning\nSuizhou\nEnshi\nXiantao\nQianjiang\nTianmen\nShennongjia",                       // 17
    "Changsha\nZhuzhou\nXiangtan\nHengyang\nShaoyang\nYueyang\nChangde\nZhangjiajie\nYiyang\nChenzhou\nYongzhou\nHuaihua\nLoudi\nXiangxi",                                                 // 14
    "Guangzhou\nShaoguan\nShenzhen\nZhuhai\nShantou\nFoshan\nJiangmen\nZhanjiang\nMaoming\nZhaoqing\nHuizhou\nMeizhou\nShanwei\nHeyuan\nYangjiang\nQingyuan\nDongguan\nZhongshan\nChaozhou\nJieyang\nYunfu", // 21
    "Nanning\nLiuzhou\nGuilin\nWuzhou\nBeihai\nFangchenggang\nQinzhou\nGuigang\nYulin\nBaise\nHezhou\nHechi\nLaibin\nChongzuo",                                                             // 14
    "Haikou\nSanya\nSansha\nDanzhou\nWuzhishan\nQionghai\nWenchang\nWanning\nDongfang\nDing'an\nTunchang\nChengmai\nLingao\nBaisha\nChangjiang\nLedong\nLingshui\nBaoting\nQiongzhong",     // 19
    "Chongqing\nYuzhong\nDadukou\nJiangbei\nShapingba\nJiulongpo\nNan'an\nKaizhou\n",                                                                                                     // 8
    "Chengdu\nZigong\nPanzhihua\nLuzhou\nDeyang\nMianyang\nGuangyuan\nSuining\nNeijiang\nLeshan\nNanchong\nMeishan\nYibin\nGuang'an\nDazhou\nYa'an\nBazhong\nZiyang\nAba\nGanzi\nLiangshan", // 21
    "Guiyang\nLiupanshui\nZunyi\nAnshun\nBijie\nTongren\nSouth Guizhou\nSoutheast Guizhou\nSouthwest Guizhou",                                                                           // 9
    "Kunming\nQujing\nYuxi\nBaoshan\nZhaotong\nLijiang\nPuer\nLincang\nChuxiong\nHonghe\nWenshan\nXishuangbanna\nDali\nDehong\nNujiang\nDiqing",                                             // 16
    "Lhasa\nXigazê\nQamdo\nNyingchi\nShannan\nNagqu\nNgari",                                                                                                                             // 7
    "Xi'an\nTongchuan\nBaoji\nYangling\nXianyang\nWeinan\nYan'an\nHanzhong\nYulin\nAnkang\nShangluo",                                                                                     // 11
    "Lanzhou\nJiayuguan\nJinchang\nBaiyin\nTianshui\nWuwei\nZhangye\nPingliang\nJiuquan\nQingyang\nDingxi\nLongnan\nLinxia\nGannan",                                                       // 14
    "Xining\nHaidong\nHaibei\nHuangnan\nHainan\nGolog\nYushu\nHaixi",                                                                                                                     // 8
    "Yinchuan\nShizuishan\nWuzhong\nGuyuan\nZhongwei\n",                                                                                                                                 // 5
    "Urumqi\nKaramay\nTurpan\nHami\nChangji\nBortala\nBayingolin\nAksu\nKizilsu\nKashgar\nHotan\nIli\nTacheng\nAltay\nShihezi\nAlar\nTumushuke\nWujiaqu\nBeitun\nTiemenguan\nShuanghe\nKokdala\nKunyu", // 23
    "Taipei\nKaohsiung\nTaichung",
    "Hong Kong",
    "Macao"};
city_t my_city_3[400] = {
    // 北京0
    "北京\n东城\n朝阳\n丰台\n石景山\n海淀\n门头沟\n房山\n通州\n顺义\n昌平\n大兴\n怀柔\n平谷\n密云\n延庆",
    "西城",
    // 天津1
    "天津\n东丽\n西青\n津南\n北辰\n武清\n宝坻\n滨海新区\n宁河\n静海\n蓟州",
    "和平",
    "河东",
    "河西",
    "南开",
    "河北区",
    "红桥",
    // 河北2
    "石家庄\n长安\n桥西\n新华\n井陉矿\n裕华\n藁城\n鹿泉\n栾城\n井陉\n正定\n行唐\n灵寿\n高邑\n深泽\n赞皇\n无极\n平山\n元氏\n赵县\n辛集\n晋州\n新乐",
    "唐山\n路南\n路北\n古冶\n开平\n丰南\n丰润\n曹妃甸\n滦南\n乐亭\n迁西\n玉田\n遵化\n迁安\n滦州",
    "秦皇岛\n海港\n山海关\n北戴河\n抚宁\n青龙\n昌黎\n卢龙",
    "邯郸\n邯山\n丛台\n复兴\n峰峰\n肥乡\n永年\n临漳\n成安\n大名\n涉县\n磁县\n邱县\n鸡泽\n广平\n馆陶\n魏县\n曲周\n武安",
    "邢台\n桥东\n信都\n临城\n内丘\n柏乡\n隆尧\n任泽\n南和\n宁晋\n巨鹿\n新河\n广宗\n平乡\n威县\n清河\n临西\n南宫\n沙河",
    "保定\n竞秀\n莲池\n满城\n清苑\n徐水\n涞水\n阜平\n定兴\n唐县\n高阳\n容城\n涞源\n望都\n安新\n易县\n曲阳\n蠡县\n顺平\n博野\n雄县\n涿州\n定州\n安国\n高碑店",
    "张家口\n襄都\n桥西\n宣化\n下花园\n万全\n崇礼\n张北\n康保\n沽源\n尚义\n蔚县\n阳原\n怀安\n怀来\n涿鹿\n赤城",
    "承德\n双桥\n双滦\n鹰手营子矿\n承德县\n兴隆\n滦平\n隆化\n丰宁\n宽城\n围场\n平泉",
    "沧州\n新华\n运河\n沧县\n青县\n东光\n海兴\n盐山\n肃宁\n南皮\n吴桥\n献县\n孟村\n泊头\n任丘\n黄骅\n河间",
    "廊坊\n安次\n广阳\n固安\n永清\n香河\n大城\n文安\n大厂\n霸州\n三河",
    "衡水\n桃城\n冀州\n枣强\n武邑\n武强\n饶阳\n安平\n故城\n景县\n阜城\n深州",
    /// 山西3
    "太原\n小店区\n迎泽\n杏花岭\n尖草坪区\n万柏林\n晋源\n清徐\n阳曲\n娄烦\n古交",
    "大同\n矿区\n南郊\n新荣\n大同县\n阳高\n天镇\n广灵\n灵丘\n浑源\n左云",
    "阳泉\n矿区\n郊区\n平定\n盂县",
    "长治\n屯留\n潞城\n潞州\n襄垣\n平顺\n黎城\n壶关\n长子\n武乡\n沁县\n沁源",
    "晋城\n沁水\n阳城\n陵川\n泽州\n高平",
    "朔州\n朔城\n平鲁\n山阴\n应县\n右玉\n怀仁",
    "晋中\n榆次\n榆社\n左权\n和顺\n昔阳\n寿阳\n太谷\n祁县\n平遥\n灵石\n介休",
    "运城\n盐湖\n临猗\n万荣\n闻喜\n稷山\n新绛\n绛县\n垣曲\n夏县\n平陆\n芮城\n永济\n河津",
    "忻州\n忻府\n定襄\n五台县\n代县\n繁峙\n宁武\n静乐\n神池\n五寨\n岢岚\n河曲\n保德\n偏关\n五台山\n原平",
    "临汾\n尧都\n曲沃\n翼城\n襄汾\n洪洞\n古县\n安泽\n浮山\n吉县\n乡宁\n大宁\n隰县\n永和\n蒲县\n汾西\n侯马\n霍州",
    "吕梁\n离石\n文水\n交城\n兴县\n临县\n柳林\n石楼\n岚县\n方山\n中阳\n交口\n孝义\n汾阳",
    /// 内蒙古4
    "呼和浩特\n新城\n回民\n玉泉\n赛罕\n土左旗\n托县\n和林\n清水河\n武川",
    "包头\n东河\n昆都仑\n青山\n石拐\n白云鄂博\n九原\n土右旗\n固阳\n达茂旗\n满都拉\n希拉穆仁",
    "乌海\n海勃湾\n海南\n乌达",
    "赤峰\n红山\n元宝山\n松山\n岗子\n阿鲁旗\n巴林左旗\n巴林右旗\n林西\n克什克腾\n翁牛特\n喀喇沁\n宁城\n八里罕\n敖汉\n宝国吐",
    "通辽\n科尔沁\n科左中旗\n舍伯吐\n科左后旗\n开鲁\n库伦\n奈曼\n青龙山\n扎鲁特\n巴雅尔吐胡硕\n霍林郭勒",
    "鄂尔多斯\n东胜\n康巴什\n达拉特\n准格尔\n鄂前旗\n鄂托克\n杭锦旗\n伊和乌素\n乌审旗\n乌审召\n河南\n伊金霍洛",
    "呼伦贝尔\n海拉尔\n扎赉诺尔\n阿荣旗\n莫力达瓦\n博克图\n鄂伦春旗\n小二沟\n图里河\n鄂温克旗\n陈旗\n新左旗\n新右旗\n满洲里\n牙克石\n扎兰屯\n额尔古纳\n根河",
    "巴彦淖尔\n临河\n五原\n磴口\n那仁宝力格\n乌前旗\n大佘太\n乌中旗\n乌后旗\n杭锦后旗\n海力素",
    "乌兰察布\n集宁\n卓资\n化德\n商都\n兴和\n凉城\n察右前旗\n察右中旗\n察右后旗\n四子王旗\n丰镇",
    "兴安盟\n乌兰浩特\n阿尔山\n科右前旗\n索伦\n科右中旗\n扎赉特\n胡尔勒\n突泉",
    "锡林郭勒\n二连浩特\n锡林浩特\n阿巴嘎\n苏左旗\n苏右旗\n朱日和\n东乌旗\n西乌旗\n太仆寺\n镶黄旗\n正镶白旗\n正蓝旗\n多伦\n乌拉盖",
    "阿拉善盟\n阿左旗\n吉兰太\n巴彦诺日公\n阿右旗\n雅布赖\n额济纳\n拐子湖\n乌斯太\n孪井滩",
    // 辽宁
    "沈阳\n和平\n沈河\n大东\n皇姑\n铁西\n苏家屯\n浑南\n沈北新区\n于洪\n辽中\n康平\n法库\n新民",
    "大连\n中山\n西岗\n沙河口\n甘井子\n旅顺口\n金州\n普兰店\n长海\n瓦房店\n庄河",
    "鞍山\n铁东\n铁西\n立山\n千山\n台安\n岫岩\n海城",
    "抚顺\n新抚\n东洲\n望花\n顺城\n新宾\n清原",
    "本溪\n平山\n溪湖\n明山\n南芬\n本溪县\n桓仁",
    "丹东\n元宝\n振兴\n振安\n宽甸\n东港\n凤城",
    "锦州\n古塔\n凌河\n太和\n黑山\n义县\n凌海\n镇",
    "营口\n站前\n西市\n鲅鱼圈\n老边\n盖州\n大桥",
    "阜新\n海州\n新邱\n太平\n清河门\n细河\n彰武",
    "辽阳\n白塔\n文圣\n宏伟\n弓长岭\n太子河\n辽阳\n灯塔",
    "盘锦\n双台子\n兴隆台\n大洼\n盘山",
    "铁岭\n银州\n清河\n西丰\n昌图\n调兵山\n开原",
    "朝阳\n双塔\n龙城\n建平\n喀左\n北\n凌源",
    "葫芦岛\n连山\n龙港\n南票\n绥中\n建昌\n兴城",
    // 吉林
    "长春\n南关\n宽城\n朝阳\n二道\n绿园\n双阳\n九台\n农安\n榆树\n德惠\n公主岭",
    "吉林\n昌邑\n龙潭\n船营\n丰满\n永吉\n蛟河\n桦甸\n舒兰\n磐石",
    "四平\n铁西\n铁东\n梨树\n伊通\n双辽",
    "辽源\n龙山\n西安\n东丰\n东辽",
    "通化\n东昌\n二道江\n通化县\n辉南\n柳河\n梅河口\n集安",
    "白山\n浑江\n江源\n抚松\n东岗\n靖宇\n长白\n临江",
    "松原\n宁江\n前郭\n长岭\n乾安\n扶余",
    "白城\n洮北\n镇赉\n通榆\n洮南\n大安",
    "延边\n延吉\n图们\n敦化\n珲春\n龙井\n和龙\n汪清\n安图",
    // 黑龙江
    "哈尔滨\n道里\n南岗\n道外\n平房\n松北\n香坊\n呼兰\n阿城\n双城\n依兰\n方正\n宾县\n巴彦\n木兰\n通河\n延寿\n尚志\n五常",
    "齐齐哈尔\n龙沙\n建华\n铁锋\n昂昂溪\n富拉尔基\n碾子山\n梅里斯\n龙江\n依安\n泰来\n甘南\n富裕\n克山\n克东\n拜泉\n讷河",
    "鸡西\n鸡冠\n恒山\n滴道\n梨树\n城子河\n麻山\n鸡东\n虎林\n密山",
    "鹤岗\n向阳\n工农\n南山\n兴安\n东山\n兴山\n萝北\n绥滨",
    "双鸭山\n尖山\n岭东\n四方台\n宝山\n集贤\n友谊\n宝清\n饶河",
    "大庆\n萨尔图\n龙凤\n让胡路\n红岗\n大同\n肇州\n肇源\n林甸\n杜尔伯特",
    "伊春\n西林\n翠峦\n新青\n伊美\n金山屯\n五营\n乌马河\n汤旺\n带岭\n乌伊岭\n红星\n上甘岭\n友好\n嘉荫\n南岔\n铁力",
    "佳木斯\n向阳\n前进\n东风\n郊区\n桦南\n桦川\n汤原\n同江\n富锦\n抚远",
    "七台河\n新兴\n桃山\n茄子河\n勃利",
    "牡丹江\n东安\n阳明\n爱民\n西安\n林口\n绥芬河\n海林\n宁安\n穆棱\n东宁",
    "黑河\n爱辉\n嫩江\n逊克\n孙吴\n北安\n五大连池",
    "绥化\n北林\n望奎\n兰西\n青冈\n庆安\n明水\n绥棱\n安达\n肇东\n海伦",
    "大兴安岭\n漠河\n呼玛\n塔河\n加格达奇\n新林\n呼中",
    // 上海
    "上海\n徐汇\n闵行\n宝山\n嘉定\n浦东新区\n金山\n松江\n青浦\n奉贤\n崇明",
    "黄浦",
    "长宁",
    "静安",
    "普陀",
    "虹口",
    "杨浦",
    // 江苏
    "南京\n玄武\n秦淮\n建邺\n鼓楼\n浦口\n栖霞\n雨花台\n江宁\n六合\n溧水\n高淳",
    "无锡\n锡山\n惠山\n滨湖\n梁溪\n新吴\n江阴\n宜兴",
    "徐州\n鼓楼\n云龙\n贾汪\n泉山\n铜山\n丰县\n沛县\n睢宁\n新沂\n邳州",
    "常州\n天宁\n钟楼\n新北\n武进\n溧阳\n金坛",
    "苏州\n虎丘\n吴中\n相城\n姑苏\n吴江\n常熟\n张家港\n昆山\n太仓",
    "南通\n崇川\n港闸\n通州\n如东\n启东\n如皋\n海门\n海安",
    "连云港\n连云\n海州\n赣榆\n东海\n灌云\n灌南",
    "淮安\n淮安区\n淮阴区\n清江浦\n洪泽\n涟水\n盱眙\n金湖",
    "盐城\n亭湖\n盐都\n大丰\n响水\n滨海\n阜宁\n射阳\n建湖\n东台",
    "扬州\n广陵\n邗江\n江都\n宝应\n仪征\n高邮",
    "镇江\n京口\n润州\n丹徒\n丹阳\n扬中\n句容",
    "泰州\n海陵\n高港\n姜堰\n兴化\n靖江\n泰兴",
    "宿迁\n宿城\n宿豫\n沭阳\n泗阳\n泗洪",
    // 浙江
    "杭州\n上城\n下城\n江干\n拱墅\n西湖\n滨江\n萧山\n余杭\n桐庐\n淳安\n建德\n富阳\n临安",
    "宁波\n海曙\n江北\n北仑\n镇海\n鄞州\n奉化\n象山\n宁海\n余姚\n慈溪",
    "温州\n鹿城\n龙湾\n瓯海\n洞头\n永嘉\n平阳\n苍南\n文成\n泰顺\n瑞安\n乐清",
    "嘉兴\n南湖\n秀洲\n嘉善\n海盐\n海宁\n平湖\n桐乡",
    "湖州\n吴兴\n南浔\n德清\n长兴\n安吉",
    "绍兴\n越城\n柯桥\n上虞\n新昌\n诸暨\n嵊州",
    "金华\n婺城\n金东\n武义\n浦江\n磐安\n兰溪\n义乌\n东阳\n永康",
    "衢州\n柯城\n衢江\n常山\n开化\n龙游\n江山",
    "舟山\n定海\n普陀\n岱山\n嵊泗",
    "台州\n椒江\n黄岩\n路桥\n三门\n天台\n仙居\n温岭\n临海\n玉环",
    "丽水\n莲都\n青田\n缙云\n遂昌\n松阳\n云和\n庆元\n景宁\n龙泉",
    // 安徽
    "合肥\n瑶海\n庐阳\n蜀山\n包河\n长丰\n肥东\n肥西\n庐江\n巢湖",
    "芜湖\n镜湖\n弋江\n鸠江\n三山\n芜湖县\n繁昌\n南陵\n无为",
    "蚌埠\n龙子湖\n蚌山\n禹会\n淮上\n怀远\n五河\n固镇",
    "淮南\n大通\n田家庵\n谢家集\n八公山\n潘集\n凤台\n寿县",
    "马鞍山\n花山\n雨山\n博望\n当涂\n含山\n和县",
    "淮北\n杜集\n相山\n烈山\n濉溪",
    "铜陵\n铜官\n义安\n郊区\n枞阳",
    "安庆\n迎江\n大观\n宜秀\n怀宁\n太湖\n宿松\n望江\n岳西\n桐城\n潜山",
    "黄山\n屯溪\n黄山区\n黄山风景区(光明顶)\n徽州\n歙县\n休宁\n黟县\n祁门",
    "滁州\n琅琊\n南谯\n来安\n全椒\n定远\n凤阳\n天长\n明光",
    "阜阳\n颍州\n颍东\n颍泉\n临泉\n太和\n阜南\n颍上\n界首",
    "宿州\n埇桥\n砀山\n萧县\n灵璧\n泗县",
    "六安\n金安\n裕安\n叶集\n霍邱\n舒城\n金寨\n霍山",
    "亳州\n谯城\n涡阳\n蒙城\n利辛",
    "池州\n贵池\n东至\n石台\n青阳\n九华山",
    "宣城\n宣州\n郎溪\n广德\n泾县\n绩溪\n旌德\n宁国",
    // 福建
    "福州\n鼓楼\n台江\n仓山\n马尾\n晋安\n长乐\n闽侯\n连江\n罗源\n闽清\n永泰\n平潭\n福清",
    "厦门\n思明\n海沧\n湖里\n集美\n同安\n翔安",
    "莆田\n城厢\n涵江\n荔城\n秀屿\n仙游",
    "三明\n梅列\n三元\n明溪\n清流\n宁化\n大田\n尤溪\n沙县\n将乐\n泰宁\n建宁\n永安",
    "泉州\n鲤城\n丰泽\n洛江\n泉港\n惠安\n崇武\n安溪\n永春\n德化\n金门\n石狮\n晋江\n南安",
    "漳州\n芗城\n龙文\n云霄\n漳浦\n诏安\n长泰\n东山\n南靖\n平和\n华安\n龙海",
    "南平\n延平\n建阳\n顺昌\n浦城\n光泽\n松溪\n政和\n邵武\n武夷山\n建瓯",
    "龙岩\n新罗\n永定\n长汀\n上杭\n武平\n连城\n漳平",
    "宁德\n蕉城\n霞浦\n古田\n屏南\n寿宁\n周宁\n柘荣\n福安\n福鼎",
    // 江西
    "南昌\n东湖\n西湖\n青云谱\n湾里\n青山湖\n新建\n南昌县\n安义\n进贤",
    "景德镇\n昌江\n珠山\n浮梁\n乐平",
    "萍乡\n安源\n湘东\n莲花\n上栗\n芦溪",
    "九江\n濂溪\n浔阳\n柴桑\n武宁\n修水\n永修\n德安\n都昌\n湖口\n彭泽\n瑞昌\n共青城\n庐山",
    "新余\n渝水\n分宜",
    "鹰潭\n月湖\n余江\n贵溪",
    "赣州\n章贡\n南康\n赣县\n信丰\n大余\n上犹\n崇义\n安远\n龙南\n定南\n全南\n宁都\n于都\n兴国\n会昌\n寻乌\n石城\n瑞金",
    "吉安\n吉州\n青原\n吉安县\n吉水\n峡江\n新干\n永丰\n泰和\n遂川\n万安\n安福\n永新\n井冈山",
    "宜春\n袁州\n奉新\n万载\n上高\n宜丰\n靖安\n铜鼓\n丰城\n樟树\n高安",
    "抚州\n临川\n东乡\n南城\n黎川\n南丰\n崇仁\n乐安\n宜黄\n金溪\n资溪\n广昌",
    "上饶\n信州\n广丰\n上饶县\n玉山\n铅山\n横峰\n弋阳\n余干\n鄱阳\n万年\n婺源\n德兴",
    // 山东
    "济南\n历下\n市中\n槐荫\n天桥\n历城\n长清\n济阳\n莱芜\n平阴\n商河\n章丘",
    "钢城\n莱城",
    "青岛\n市南\n市北\n黄岛\n崂山\n李沧\n城阳\n胶州\n即墨\n平度\n莱西",
    "淄博\n淄川\n张店\n博山\n临淄\n周村\n桓台\n高青\n沂源",
    "枣庄\n市中\n薛城\n峄城\n台儿庄\n山亭\n滕州",
    "东营\n河口\n垦利\n利津\n广饶",
    "烟台\n芝罘\n福山\n牟平\n莱山\n长岛\n龙口\n莱阳\n莱州\n蓬莱\n招远\n栖霞\n海阳",
    "潍坊\n潍城\n寒亭\n坊子\n奎文\n临朐\n昌乐\n青州\n诸城\n寿光\n安丘\n高密\n昌邑",
    "济宁\n任城\n兖州\n微山\n鱼台\n金乡\n嘉祥\n汶上\n泗水\n梁山\n曲阜\n邹城",
    "泰安\n泰山\n岱岳\n宁阳\n东平\n新泰\n肥城",
    "威海\n环翠\n文登\n荣成\n乳山\n石岛\n成山头",
    "日照\n东港\n岚山\n五莲\n莒县",
    "临沂\n兰山\n罗庄\n河东\n沂南\n郯城\n沂水\n兰陵\n费县\n平邑\n莒南\n蒙阴\n临沭",
    "德州\n德城\n陵城\n宁津\n庆云\n临邑\n齐河\n平原\n夏津\n武城\n乐陵\n禹城",
    "聊城\n东昌府\n阳谷\n莘县\n茌平\n东阿\n冠县\n高唐\n临清",
    "滨州\n滨城\n沾化\n惠民\n阳信\n无棣\n博兴\n邹平",
    "菏泽\n牡丹\n定陶\n曹县\n单县\n成武\n巨野\n郓城\n鄄城\n东明",
    // 河南
    "郑州\n中原\n二七\n管城\n金水\n上街\n惠济\n中牟\n巩义\n荥阳\n新密\n新郑\n登封",
    "开封\n龙亭\n顺河\n鼓楼\n禹王台\n祥符\n杞县\n通许\n尉氏\n兰考",
    "洛阳\n老城\n西工\n瀍河\n涧西\n吉利\n洛龙\n孟津\n新安\n栾川\n嵩县\n汝阳\n宜阳\n洛宁\n伊川\n偃师",
    "平顶山\n新华\n卫东\n石龙\n湛河\n宝丰\n叶县\n鲁山\n郏县\n舞钢\n汝州",
    "安阳\n文峰\n北关\n殷都\n龙安\n汤阴\n滑县\n内黄\n林州",
    "鹤壁\n鹤山\n山城\n淇滨\n浚县\n淇县",
    "新乡\n红旗\n卫滨\n凤泉\n牧野\n获嘉\n原阳\n延津\n封丘\n长垣\n卫辉\n辉县",
    "焦作\n解放\n中站\n马村\n山阳\n修武\n博爱\n武陟\n温县\n沁阳\n孟州",
    "濮阳\n华龙\n清丰\n南乐\n范县\n台前",
    "许昌\n魏都\n建安\n鄢陵\n襄城\n禹州\n长葛",
    "漯河\n源汇\n郾城\n召陵\n舞阳\n临颍",
    "三门峡\n湖滨\n陕州\n渑池\n卢氏\n义马\n灵宝",
    "南阳\n宛城\n卧龙\n南召\n方城\n西峡\n镇平\n内乡\n淅川\n社旗\n唐河\n新野\n桐柏\n邓州",
    "商丘\n梁园\n睢阳\n民权\n睢县\n宁陵\n柘城\n虞城\n夏邑\n永城",
    "信阳\n浉河\n平桥\n罗山\n光山\n新县\n商城\n固始\n潢川\n淮滨\n息县",
    "周口\n川汇\n扶沟\n西华\n商水\n沈丘\n郸城\n淮阳\n太康\n鹿邑\n项城",
    "驻马店\n驿城\n西平\n上蔡\n平舆\n正阳\n确山\n泌阳\n汝南\n遂平\n新蔡",
    "济源",
    // 湖北
    "武汉\n江岸\n江汉\n硚口\n汉阳\n武昌\n青山\n洪山\n东西湖\n汉南\n蔡甸\n江夏\n黄陂\n新洲",
    "黄石\n黄石港\n西塞山\n下陆\n铁山\n阳新\n大冶",
    "十堰\n茅箭\n张湾\n郧阳\n郧西\n竹山\n竹溪\n房县\n丹江口",
    "宜昌\n西陵\n伍家岗\n点军\n猇亭\n夷陵\n三峡\n远安\n兴山\n秭归\n长阳\n五峰\n宜都\n当阳\n枝江",
    "襄阳\n襄城\n樊城\n襄州\n南漳\n谷城\n保康\n老河口\n枣阳\n宜城",
    "鄂州\n梁子湖\n华容\n鄂城",
    "荆门\n东宝\n掇刀\n沙洋\n钟祥\n京山",
    "孝感\n孝南\n孝昌\n大悟\n云梦\n应城\n安陆\n汉川",
    "荆州\n沙市\n公安\n监利\n江陵\n石首\n洪湖\n松滋",
    "黄冈\n黄州\n团风\n红安\n罗田\n英山\n浠水\n蕲春\n黄梅\n麻城\n武穴",
    "咸宁\n咸安\n嘉鱼\n通城\n崇阳\n通山\n赤壁",
    "随州\n曾都\n随县\n广水",
    "恩施\n利川\n建始\n巴东\n宣恩\n咸丰\n来凤\n鹤峰",
    "仙桃",
    "潜江",
    "天门",
    "神农架",
    // 湖南
    "长沙\n芙蓉\n天心\n岳麓\n开福\n雨花\n望城\n长沙县\n浏阳\n宁乡\n湘江新区",
    "株洲\n荷塘\n芦淞\n石峰\n天元\n攸县\n茶陵\n炎陵\n醴陵",
    "湘潭\n雨湖\n岳塘\n湘乡\n韶山",
    "衡阳\n珠晖\n雁峰\n石鼓\n蒸湘\n南岳\n衡阳县\n衡南\n衡山\n衡东\n祁东\n耒阳\n常宁",
    "邵阳\n双清\n大祥\n北塔\n邵东\n新邵\n邵阳县\n隆回\n洞口\n绥宁\n新宁\n城步\n武冈",
    "岳阳\n岳阳楼区\n云溪\n君山\n华容\n湘阴\n平江\n汨罗\n临湘",
    "常德\n武陵\n鼎城\n安乡\n汉寿\n澧县\n临澧\n桃源\n石门\n津市",
    "张家界\n永定\n武陵源\n慈利\n桑植",
    "益阳\n资阳\n赫山区\n南县\n桃江\n安化\n沅江",
    "郴州\n北湖\n苏仙\n桂阳\n宜章\n永兴\n嘉禾\n临武\n汝城\n桂东\n安仁\n资兴",
    "永州\n零陵\n冷水滩\n祁阳\n东安\n双牌\n道县\n江永\n宁远\n蓝山\n新田\n江华",
    "怀化\n鹤城\n中方\n沅陵\n辰溪\n溆浦\n会同\n麻阳\n新晃\n芷江\n靖州\n通道\n洪江",
    "娄底\n娄星\n双峰\n新化\n冷水江\n涟源",
    "湘西\n吉首\n泸溪\n凤凰\n花垣\n保靖\n古丈\n永顺\n龙山",
    // 广东
    "广州\n荔湾\n越秀\n海珠\n天河\n白云\n黄埔\n番禺\n花都\n南沙\n从化\n增城",
    "韶关\n武江\n浈江\n曲江\n始兴\n仁化\n翁源\n乳源\n新丰\n乐昌\n南雄",
    "深圳\n罗湖\n福田\n南山\n宝安\n龙岗\n盐田\n龙华\n坪山\n光明",
    "珠海\n香洲\n斗门\n金湾",
    "汕头\n龙湖\n金平\n濠江\n潮阳\n潮南\n澄海\n南澳",
    "佛山\n禅城\n南海\n顺德\n三水\n高明",
    "江门\n蓬江\n江海\n新会\n台山\n开平\n鹤山\n恩平",
    "湛江\n赤坎\n霞山\n坡头\n麻章\n遂溪\n徐闻\n廉江\n雷州\n吴川",
    "茂名\n茂南\n电白\n高州\n化州\n信宜",
    "肇庆\n端州\n鼎湖\n高要\n广宁\n怀集\n封开\n德庆\n四会",
    "惠州\n惠城\n惠阳\n博罗\n惠东\n龙门",
    "梅州\n梅江\n梅县\n大埔\n丰顺\n五华\n平远\n蕉岭\n兴宁",
    "汕尾\n海丰\n陆河\n陆丰",
    "河源\n源城\n紫金\n龙川\n连平\n和平\n东源",
    "阳江\n江城\n阳东\n阳西\n阳春",
    "清远\n清城\n清新\n佛冈\n阳山\n连山\n连南\n英德\n连州",
    "东莞",
    "中山",
    "潮州\n湘桥\n潮安\n饶平",
    "揭阳\n榕城\n揭东\n揭西\n惠来\n普宁",
    "云浮\n云城\n云安\n新兴\n郁南\n罗定",
    // 广西
    "南宁\n兴宁\n青秀\n江南\n西乡塘\n良庆\n邕宁\n武鸣\n隆安\n马山\n上林\n宾阳\n横州",
    "柳州\n城中\n鱼峰\n柳南\n柳北\n柳江\n柳城\n鹿寨\n融安\n融水\n三江",
    "桂林\n秀峰\n叠彩\n象山\n七星\n雁山\n临桂\n阳朔\n灵川\n全州\n兴安\n永福\n灌阳\n龙胜\n资源\n平乐\n恭城\n荔浦",
    "梧州\n万秀\n长洲\n龙圩\n苍梧\n藤县\n蒙山\n岑溪",
    "北海\n海城\n涠洲岛\n银海\n铁山港\n合浦",
    "防城港\n港口\n防城\n上思\n东兴",
    "钦州\n钦南\n钦北\n灵山\n浦北",
    "贵港\n港北\n港南\n覃塘\n平南\n桂平",
    "玉林\n玉州\n福绵\n容县\n陆川\n博白\n兴业\n北流",
    "百色\n右江\n田阳\n田东\n平果\n德保\n那坡\n凌云\n乐业\n田林\n西林\n隆林\n靖西",
    "贺州\n八步\n平桂\n昭平\n钟山\n富川",
    "河池\n金城江\n宜州\n南丹\n天峨\n凤山\n东兰\n罗城\n环江\n巴马\n都安\n大化",
    "来宾\n兴宾\n忻城\n象州\n武宣\n金秀\n合山",
    "崇左\n江州\n扶绥\n宁明\n龙州\n大新\n天等\n凭祥",
    // 海南
    "海口\n秀英\n龙华\n琼山\n美兰",
    "三亚\n海棠\n吉阳\n天涯\n崖州",
    "三沙\n西沙\n南沙\n中沙",
    "儋州",
    "五指山",
    "琼海",
    "文昌",
    "万宁",
    "东方",
    "定安",
    "屯昌",
    "澄迈",
    "临高",
    "白沙",
    "昌江",
    "乐东",
    "陵水",
    "保亭",
    "琼中",
    // 重庆
    "重庆\n万州\n涪陵\n北碚\n綦江\n大足\n渝北\n巴南\n黔江\n长寿\n江津\n合川\n永川\n南川\n璧山\n铜梁\n潼南\n荣昌\n梁平\n武隆\n城口\n丰都\n垫江\n忠县\n云阳\n奉节\n巫山\n巫溪\n石柱\n秀山\n酉阳\n彭水",
    "渝中",
    "大渡口",
    "江北",
    "沙坪坝",
    "九龙坡",
    "南岸",
    "开州",
    // 四川
    "成都\n锦江\n青羊\n金牛\n武侯\n成华\n龙泉驿\n青白江\n新都\n温江\n双流\n郫都\n金堂\n大邑\n蒲江\n新津\n都江堰\n彭州\n邛崃\n崇州\n简阳",
    "自贡\n自流井\n贡井\n大安\n沿滩\n荣县\n富顺",
    "攀枝花\n东区\n西区\n仁和\n米易\n盐边",
    "泸州\n江阳\n纳溪\n龙马潭\n泸县\n合江\n叙永\n古蔺",
    "德阳\n旌阳\n罗江\n中江\n广汉\n什邡\n绵竹",
    "绵阳\n涪城\n游仙\n安州\n三台\n盐亭\n梓潼\n北川\n平武\n江油",
    "广元\n利州\n昭化\n朝天\n旺苍\n青川\n剑阁\n苍溪",
    "遂宁\n船山\n安居\n蓬溪\n射洪\n大英",
    "内江\n市中\n东兴\n威远\n资中\n隆昌",
    "乐山\n市中\n沙湾\n五通桥\n金口河\n犍为\n井研\n夹江\n沐川\n峨边\n马边\n峨眉山",
    "南充\n顺庆\n高坪\n嘉陵\n南部\n营山\n蓬安\n仪陇\n西充\n阆中",
    "眉山\n东坡\n彭山\n仁寿\n洪雅\n丹棱\n青神",
    "宜宾\n翠屏\n南溪\n宜宾县\n江安\n长宁\n高县\n珙县\n筠连\n兴文\n屏山",
    "广安\n前锋\n岳池\n武胜\n邻水\n华蓥",
    "达州\n通川\n达川\n宣汉\n开江\n大竹\n渠县\n万源",
    "雅安\n雨城\n名山\n荥经\n汉源\n石棉\n天全\n芦山\n宝兴",
    "巴中\n巴州\n恩阳\n通江\n南江\n平昌",
    "资阳\n雁江\n安岳\n乐至",
    "阿坝\n马尔康\n汶川\n理县\n茂县\n松潘\n九寨沟\n金川\n小金\n黑水\n壤塘\n若尔盖\n红原",
    "甘孜\n康定\n泸定\n丹巴\n九龙\n雅江\n道孚\n炉霍\n新龙\n德格\n白玉\n石渠\n色达\n理塘\n巴塘\n乡城\n稻城\n得荣",
    "凉山\n西昌\n木里\n盐源\n德昌\n会理\n会东\n宁南\n普格\n布拖\n金阳\n昭觉\n喜德\n冕宁\n越西\n甘洛\n美姑\n雷波",
    // 贵州
    "贵阳\n南明\n云岩\n花溪\n乌当\n白云\n观山湖\n开阳\n息烽\n修文\n清镇",
    "六盘水\n钟山\n六枝\n水城\n盘州",
    "遵义\n红花岗\n汇川\n播州\n桐梓\n绥阳\n正安\n道真\n务川\n凤冈\n湄潭\n余庆\n习水\n赤水\n仁怀",
    "安顺\n西秀\n平坝\n普定\n镇宁\n关岭\n紫云",
    "毕节\n七星关\n大方\n黔西\n金沙\n织金\n纳雍\n威宁\n赫章",
    "铜仁\n碧江\n万山\n江口\n玉屏\n石阡\n思南\n印江\n德江\n沿河\n松桃",
    "黔西南\n兴义\n兴仁\n普安\n晴隆\n贞丰\n望谟\n册亨\n安龙",
    "黔东南\n凯里\n黄平\n施秉\n三穗\n镇远\n岑巩\n天柱\n锦屏\n剑河\n台江\n黎平\n榕江\n从江\n雷山\n麻江\n丹寨",
    "黔南\n都匀\n福泉\n荔波\n贵定\n瓮安\n独山\n平塘\n罗甸\n长顺\n龙里\n惠水\n三都",
    // 云南
    "昆明\n五华\n盘龙\n官渡\n西山\n东川\n呈贡\n晋宁\n富民\n宜良\n石林\n嵩明\n禄劝\n寻甸\n安宁",
    "曲靖\n麒麟\n沾益\n马龙\n陆良\n师宗\n罗平\n富源\n会泽\n宣威",
    "玉溪\n红塔\n江川\n澄江\n通海\n华宁\n易门\n峨山\n新平\n元江",
    "保山\n隆阳\n施甸\n龙陵\n昌宁\n腾冲",
    "昭通\n昭阳\n鲁甸\n巧家\n盐津\n大关\n永善\n绥江\n镇雄\n彝良\n威信\n水富",
    "丽江\n古城\n玉龙\n永胜\n华坪\n宁蒗",
    "普洱\n思茅\n宁洱\n墨江\n景东\n景谷\n镇沅\n江城\n孟连\n澜沧\n西盟",
    "临沧\n临翔\n凤庆\n云县\n永德\n镇康\n双江\n耿马\n沧源",
    "楚雄\n楚雄\n双柏\n牟定\n南华\n姚安\n大姚\n永仁\n元谋\n武定\n禄丰",
    "红河\n个旧\n开远\n蒙自\n弥勒\n屏边\n建水\n石屏\n泸西\n元阳\n金平\n绿春\n河口",
    "文山\n文山\n砚山\n西畴\n麻栗坡\n马关\n丘北\n广南\n富宁",
    "西双版纳\n景洪\n勐海\n勐腊",
    "大理\n大理\n漾濞\n祥云\n宾川\n弥渡\n南涧\n巍山\n永平\n云龙\n洱源\n剑川\n鹤庆",
    "德宏\n瑞丽\n芒市\n梁河\n盈江\n陇川",
    "怒江\n泸水\n福贡\n贡山\n兰坪",
    "迪庆\n香格里拉\n德钦\n维西",
    // 西藏
    "拉萨\n城关\n堆龙德庆\n达孜\n林周\n当雄\n尼木\n曲水\n墨竹工卡",
    "日喀则\n桑珠孜\n南木林\n江孜\n定日\n萨迦\n拉孜\n昂仁\n谢通门\n白朗\n仁布\n康马\n定结\n仲巴\n亚东\n帕里\n吉隆\n聂拉木\n萨嘎\n岗巴",
    "昌都\n卡若\n江达\n贡觉\n类乌齐\n丁青\n察雅\n八宿\n左贡\n芒康\n洛隆\n边坝",
    "林芝\n巴宜\n工布江达\n米林\n墨脱\n波密\n察隅\n朗县",
    "山南\n乃东\n泽当\n扎囊\n贡嘎\n桑日\n琼结\n曲松\n措美\n洛扎\n加查\n隆子\n错那\n浪卡子",
    "那曲\n色尼\n嘉黎\n比如\n聂荣\n安多\n申扎\n索县\n班戈\n巴青\n尼玛\n双湖",
    "阿里\n普兰\n札达\n噶尔\n狮泉河\n日土\n革吉\n改则\n措勤",
    // 陕西
    "西安\n新城\n碑林\n莲湖\n灞桥\n未央\n雁塔\n阎良\n临潼\n长安\n高陵\n鄠邑\n蓝田\n周至",
    "铜川\n王益\n印台\n耀州\n宜君",
    "宝鸡\n渭滨\n金台\n陈仓\n凤翔\n岐山\n扶风\n眉县\n陇县\n千阳\n麟游\n凤县\n太白",
    "杨凌",
    "咸阳\n秦都\n杨陵\n渭城\n三原\n泾阳\n乾县\n礼泉\n永寿\n长武\n旬邑\n淳化\n武功\n兴平\n彬县",
    "渭南\n临渭\n华州\n潼关\n大荔\n合阳\n澄城\n蒲城\n白水\n富平\n韩城\n华阴",
    "延安\n宝塔\n安塞\n延长\n延川\n子长\n志丹\n吴起\n甘泉\n富县\n洛川\n宜川\n黄龙\n黄陵",
    "汉中\n汉台\n南郑\n城固\n洋县\n西乡\n勉县\n宁强\n略阳\n镇巴\n留坝\n佛坪",
    "榆林\n榆阳\n横山\n府谷\n靖边\n定边\n绥德\n米脂\n佳县\n吴堡\n清涧\n子洲\n神木",
    "安康\n汉滨\n汉阴\n石泉\n宁陕\n紫阳\n岚皋\n平利\n镇坪\n旬阳\n白河",
    "商洛\n商州\n洛南\n丹凤\n商南\n山阳\n镇安\n柞水",
    // 甘肃
    "兰州\n城关\n七里河\n西固\n安宁\n红古\n永登\n皋兰\n榆中",
    "嘉峪关",
    "金昌\n金川\n永昌",
    "白银\n平川\n靖远\n会宁\n景泰",
    "天水\n秦州\n麦积\n清水\n秦安\n甘谷\n武山\n张家川",
    "武威\n凉州\n民勤\n古浪\n天祝",
    "张掖\n甘州\n肃南\n民乐\n临泽\n高台\n山丹",
    "平凉\n崆峒\n泾川\n灵台\n崇信\n庄浪\n静宁\n华亭",
    "酒泉\n肃州\n金塔\n瓜州\n肃北\n阿克塞\n玉门\n敦煌",
    "庆阳\n西峰\n庆城\n环县\n华池\n合水\n正宁\n宁县\n镇原",
    "定西\n安定\n通渭\n陇西\n渭源\n临洮\n漳县\n岷县",
    "陇南\n武都\n成县\n文县\n宕昌\n康县\n西和\n礼县\n徽县\n两当",
    "临夏\n康乐\n永靖\n广河\n和政\n东乡\n积石山",
    "甘南\n合作\n临潭\n卓尼\n舟曲\n迭部\n玛曲\n碌曲\n夏河",
    // 青海
    "西宁\n城东\n城中\n城西\n城北\n大通\n湟中\n湟源",
    "海东\n乐都\n平安\n民和\n互助\n化隆\n循化",
    "海北\n门源\n祁连\n海晏\n刚察",
    "黄南\n同仁\n尖扎\n泽库\n河南",
    "海南\n共和\n同德\n贵德\n兴海\n贵南",
    "果洛\n玛沁\n班玛\n甘德\n达日\n久治\n玛多",
    "玉树\n杂多\n称多\n治多\n囊谦\n曲麻莱",
    "海西\n格尔木\n德令哈\n茫崖\n冷湖\n乌兰\n都兰\n天峻\n大柴旦",
    // 宁夏
    "银川\n兴庆\n西夏\n金凤\n永宁\n贺兰\n灵武",
    "石嘴山\n大武口\n惠农\n平罗\n陶乐",
    "吴忠\n利通\n红寺堡\n盐池\n同心\n青铜峡",
    "固原\n原州\n西吉\n隆德\n泾源\n彭阳",
    "中卫\n沙坡头\n中宁\n海原",
    // 新疆
    "乌鲁木齐\n天山\n沙依巴克\n新市\n水磨沟\n头屯河\n达坂城\n米东\n乌鲁木齐县\n小渠子\n西白杨沟\n天池",
    "克拉玛依\n独山子\n白碱滩\n乌尔禾",
    "吐鲁番\n高昌\n鄯善\n托克逊",
    "哈密\n伊州\n巴里坤\n伊吾",
    "昌吉\n阜康\n呼图壁\n玛纳斯\n奇台\n吉木萨尔\n木垒\n蔡家湖",
    "博尔塔拉\n博乐\n阿拉山口\n精河\n温泉",
    "巴音郭楞\n库尔勒\n轮台\n尉犁\n若羌\n铁干里克\n且末\n塔中\n焉耆\n和静\n巴仑台\n巴音布鲁克\n和硕\n博湖",
    "阿克苏\n温宿\n库车\n沙雅\n新和\n拜城\n乌什\n阿瓦提\n柯坪",
    "克州\n阿图什\n阿克陶\n阿合奇\n乌恰",
    "喀什\n疏附\n疏勒\n英吉沙\n泽普\n莎车\n叶城\n麦盖提\n岳普湖\n伽师\n巴楚\n塔什库尔干",
    "和田\n墨玉\n皮山\n洛浦\n策勒\n于田\n民丰",
    "伊犁\n伊宁\n奎屯\n霍尔果斯\n伊宁县\n察布查尔\n霍城\n巩留\n新源\n昭苏\n特克斯\n尼勒克",
    "塔城\n乌苏\n额敏\n沙湾\n托里\n裕民\n和布克赛尔",
    "阿勒泰\n布尔津\n富蕴\n福海\n哈巴河\n青河\n吉木乃",
    "石河子\n炮台\n莫索湾",
    "阿拉尔",
    "图木舒克",
    "五家渠",
    "北屯",
    "铁门关",
    "双河",
    "可克达拉",
    "昆玉",
    // 台湾
    "台北\n宜兰\n桃园\n新竹",
    "高雄\n台南\n嘉义\n屏东\n台东",
    "台中\n苗栗\n彰化\n南投\n云林\n花莲",
    // 香港
    "香港\n九龙\n新界",
    // 澳门
    "澳门\n氹仔岛\n路环岛",
};

city_t my_city_3_en[400] = {
    // Beijing 0
    "Beijing\nDongcheng\nChaoyang\nFengtai\nShijingshan\nHaidian\nMentougou\nFangshan\nTongzhou\nShunyi\nChangping\nDaxing\nHuairou\nPinggu\nMiyun\nYanqing",
    "Xicheng",
    // Tianjin 1
    "Tianjin\nDongli\nXiqing\nJinnan\nBeichen\nWuqing\nBaodi\nBinhai New Area\nNinghe\nJinghai\nJizhou",
    "Heping",
    "Hedong",
    "Hexi",
    "Nankai",
    "Hebei District",
    "Hongqiao",
    // Hebei 2
    "Shijiazhuang\nChang'an\nQiaoxi\nXinhua\nJingxingkuang\nYuhua\nGaocheng\nLuquan\nLuancheng\nJingxing\nZhengding\nXingtang\nLingshou\nGaoyi\nShenze\nZanhuang\nWuji\nPingshan\nYuanshi\nZhaoxian\nXinji\nJinzhou\nXinle",
    "Tangshan\nLunan\nLubei\nGuye\nKaiping\nFengnan\nFengrun\nCaofeidian\nLuannan\nLaoting\nQianxi\nYutian\nZunhua\nQian'an\nLuanzhou",
    "Qinhuangdao\nHaigang\nShanhaiguan\nBeidaihe\nFuning\nQinglong\nChangli\nLulong",
    "Handan\nHanshan\nCongtai\nFuxing\nFengfeng\nFeixiang\nYongnian\nLinzhang\nCheng'an\nDaming\nShexian\nCixian\nQiu County\nJize\nGuangping\nGuantao\nWeixian\nQuzhou\nWuan",
    "Xingtai\nQiaodong\nXindu\nLincheng\nNeiqiu\nBaixiang\nLongyao\nRenze\nNanhe\nNingjin\nJulu\nXinhe\nGuangzong\nPingxiang\nWeixian\nQinghe\nLinxi\nNangong\nShahe",
    "Baoding\nJingxiu\nLianchi\nMancheng\nQingyuan\nXushui\nLaishui\nFuping\nDingxing\nTang County\nGao Yang\nRongcheng\nLaiyuan\nWangdu\nAnxin\nYi County\nQuyang\nLi County\nShunping\nBoye\nXiong County\nZhuozhou\nDingzhou\nAnguo\nGaobeidian",
    "Zhangjiakou\nXiangdu\nQiaoxi\nXuanhua\nXiahuayuan\nWanquan\nChongli\nZhangbei\nKangbao\nGuyuan\nShangyi\nYu County\nYangyuan\nHuai'an\nHuailai\nZhuolu\nChicheng",
    "Chengde\nShuangqiao\nShuangluan\nYingshouyingzi Kuangqu\nChengde County\nXinglong\nLuanping\nLonghua\nFengning\nKuancheng\nWeichang\nPingquan",
    "Cangzhou\nXinhua\nYunhe\nCang County\nQing County\nDongguang\nHaixing\nYanshan\nSuning\nNanpi\nWuqiao\nXian County\nMengcun\nBotou\nRenqiu\nHuanghua\nHejian",
    "Langfang\nAnci\nGuangyang\nGu'an\nYongqing\nXianghe\nDacheng\nWen'an\nDachang\nBazhou\nSanhe",
    "Hengshui\nTaocheng\nJizhou\nZaoqiang\nWuyi\nWuqiang\nRaoyang\nAnping\nGucheng\nJing County\nFucheng\nShenzhou",
    /// Shanxi 3
    "Taiyuan\nXiaodian District\nYingze\nXinghualing\nJiancaoping District\nWanbailin\nJinyuan\nQingxu\nYangqu\nLoufan\nGujiao",
    "Datong\nKuangqu\nNan Jiao\nXinrong\nDatong County\nYanggao\nTianzhen\nGuangling\nLingqiu\nHunyuan\nZuoyun",
    "Yangquan\nKuangqu\nJiaoqu\nPingding\nYu County",
    "Changzhi\nTunliu\nLucheng\nLucheng\nXiangyuan\nPingshun\nLicheng\nHuguan\nZhangzi\nWuxiang\nQin County\nQinyuan",
    "Jincheng\nQinshui\nYangcheng\nLingchuan\nZezhou\nGaoping",
    "Shuozhou\nShuocheng\nPinglu\nShanyin\nYing County\nYouyu\nHuairen",
    "Jinzhong\nYuci\nYushe\nZuoquan\nHeshun\nXiyang\nShouyang\nTaigu\nQi County\nPingyao\nLingshi\nJiexiu",
    "Yuncheng\nYanhu\nLinyi\nWanrong\nWenxi\nJishan\nXinjiang\nJiang County\nYuanqu\nXia County\nPinglu\nRuicheng\nYongji\nHejin",
    "Xinzhou\nXinfu\nDingxiang\nWutai County\nDai County\nFanshi\nNingwu\nJingle\nShenchi\nWuzhai\nKelan\nHequ\nBaode\nPianguan\nWutai Mountain\nYuanping",
    "Linfen\nYaodu\nQuwo\nYicheng\nXiangfen\nHongtong\nGu County\nAnze\nFushan\nJi County\nXiangning\nDaning\nXi County\nYonghe\nPu County\nFenxi\nHouma\nHuozhou",
    "Lvliang\nLishi\nWenshui\nJiaocheng\nXing County\nLin County\nLiulin\nShilou\nLan County\nFangshan\nZhongyang\nJiaokou\nXiaoyi\nFenyang",
    /// Inner Mongolia 4
    "Hohhot\nXincheng\nHuimin\nYuquan\nSaihan\nTumed Left Banner\nTuoketuo County\nHelin\nQingshuihe\nWuchuan",
    "Baotou\nDonghe\nHondlon\nQingshan\nShiguai\nBayan Obo\nJiuyuan\nTumed Right Banner\nGuyang\nDarhan Muminggan United Banner\nMandula\nXilamuren",
    "Wuhai\nHaibowan\nHainan\nWuda",
    "Chifeng\nHongshan\nYuanbaoshan\nSongshan\nGangzi\nAr Horqin Banner\nBairin Left Banner\nBairin Right Banner\nLinxi\nHexigten Banner\nWengniute Banner\nHarqin Banner\nNingcheng\nBalihan\nAohan Banner\nBaoguotu",
    "Tongliao\nHorqin District\nKailu\nHorqin Left Middle Banner\nShebotu\nHorqin Left Rear Banner\nKailu\nKulun Banner\nNaiman Banner\nQinglongshan\nJarud Banner\nBayartuhuxuo\nHolingol",
    "Ordos\nDongsheng\nKangbashi\nDalad Banner\nJungar Banner\nOtog Front Banner\nOtog Banner\nHanggin Banner\nYihewusu\nUxin Banner\nWushenzhao\nHenan\nEjin Horo Banner",
    "Hulunbuir\nHailar\nZhalainuoer\nArun Banner\nMorin Dawa Daur Autonomous Banner\nBoketu\nOroqen Banner\nXiao'ergou\nTulihe\nEvenk Banner\nOld Barag Banner\nNew Barag Left Banner\nNew Barag Right Banner\nManzhouli\nYakeshi\nZhalantun\nErgun\nGenhe",
    "Bayannur\nLinhe\nWuyuan\nDengkou\nNarenbaolige\nUrad Front Banner\nDa'Shetai\nUrad Middle Banner\nUrad Rear Banner\nHanggin Rear Banner\nHailisu",
    "Ulanqab\nJining\nZhuozi\nHuade\nShangdu\nXinghe\nLiangcheng\nChahar Right Front Banner\nChahar Right Middle Banner\nChahar Right Rear Banner\nSiziwang Banner\nFengzhen",
    "Hinggan League\nUlanhot\nArxan\nHorqin Right Front Banner\nSuolun\nHorqin Right Middle Banner\nJalaid Banner\nHu'erle\nTuquan",
    "Xilingol League\nErenhot\nXilinhot\nAbag Banner\nSonid Left Banner\nSonid Right Banner\nZhurihe\nEast Ujimqin Banner\nWest Ujimqin Banner\nTaibus Banner\nXianghuang Banner\nZhengxiangbai Banner\nZhenglan Banner\nDuolun\nWulagai",
    "Alxa League\nAlxa Left Banner\nJilantai\nBayannuorigong\nAlxa Right Banner\nYabulai\nEjina Banner\nGuaizi Lake\nWusitai\nLuanjingtan",
    // Liaoning
    "Shenyang\nHeping\nShenhe\nDadong\nHuanggu\nTiexi\nSujia Tun\nHunnan\nShenbei New Area\nYuhong\nLiaozhong\nKangping\nFaku\nXinmin",
    "Dalian\nZhongshan\nXigang\nShahekou\nGanjingzi\nLushunkou\nJinzhou\nPulandian\nChanghai\nWafangdian\nZhuanghe",
    "Anshan\nTiedong\nTiexi\nLishan\nQianshan\nTai'an\nXiuyan\nHaicheng",
    "Fushun\nXinfu\nDongzhou\nWanghua\nShuncheng\nXinbin\nQingyuan",
    "Benxi\nPingshan\nXihu\nMingshan\nNanfen\nBenxi County\nHuanren",
    "Dandong\nYuanbao\nZhenxing\nZhen'an\nKuandian\nDonggang\nFengcheng",
    "Jinzhou\nGuta\nLinghe\nTaihe\nHeishan\nYi County\nLinghai\nZhen",
    "Yingkou\nZhanqian\nXishi\nBayuquan\nLaobian\nGaizhou\nDaqiao",
    "Fuxin\nHaizhou\nXinqiu\nTaiping\nQinghemen\nXihe\nZhangwu",
    "Liaoyang\nBaita\nWensheng\nHongwei\nGongchangling\nTaizihe\nLiaoyang\nDengta",
    "Panjin\nShuangtaizi\nXinglongtai\nDawa\nPanshan",
    "Tieling\nYinzhou\nQinghe\nXifeng\nChangtu\nDiaobingshan\nKaiyuan",
    "Chaoyang\nShuangta\nLongcheng\nJianping\nKazuo\nBei\nLingyuan",
    "Huludao\nLianshan\nLonggang\nNanpiao\nSuizhong\nJianchang\nXingcheng",
    // Jilin
    "Changchun\nNanguan\nKuancheng\nChaoyang\nErdao\nLvyuan\nShuangyang\nJiutai\nNong'an\nYushu\nDehui\nGongzhuling",
    "Jilin\nChangyi\nLongtan\nChuanying\nFengman\nYongji\nJiaohe\nHuadian\nShulan\nPanshi",
    "Siping\nTiexi\nTiedong\nLishu\nYitong\nShuangliao",
    "Liaoyuan\nLongshan\nXi'an\nDongfeng\nDongliao",
    "Tonghua\nDongchang\nErdaojiang\nTonghua County\nHuinan\nLiuhe\nMeihekou\nJi'an",
    "Baishan\nHunjiang\nJiangyuan\nFusong\nDonggang\nJingyu\nChangbai\nLinjiang",
    "Songyuan\nNingjiang\nQianguo\nChangling\nQian'an\nFuyu",
    "Baicheng\nTaobei\nZhenlai\nTongyu\nTaonan\nDa'an",
    "Yanbian\nYanji\nTumen\nDunhua\nHunchun\nLongjing\nHelong\nWangqing\nAntu",
    // Heilongjiang
    "Harbin\nDaoli\nNangang\nDaowai\nPingfang\nSongbei\nXiangfang\nHulan\nAcheng\nShuangcheng\nYilan\nFangzheng\nBin County\nBayan\nMulan\nTonghe\nYanshou\nShangzhi\nWuchang",
    "Qiqihar\nLongsha\nJianhua\nTiefeng\nAng'angxi\nFularji\nNianzishan\nMeilisi\nLongjiang\nYi'an\nTailai\nGannan\nFuyu\nKeshan\nKedong\nBaiquan\nNehe",
    "Jixi\nJiguan\nHengshan\nDidao\nLishu\nChengzihe\nMashan\nJidong\nHulin\nMishan",
    "Hegang\nXiangyang\nGongnong\nNanshan\nXing'an\nDongshan\nXingshan\nLuobei\nSuibin",
    "Shuangyashan\nJianshan\nLingdong\nSifangtai\nBaoshan\nJixian\nYouyi\nBaoqing\nRaohe",
    "Daqing\nSartu\nLongfeng\nRanghulu\nHonggang\nDatong\nZhaozhou\nZhaoyuan\nLindian\nDorbod",
    "Yichun\nXilin\nCuiluan\nXinqing\nYimei\nJinshantun\nWuying\nWumahe\nTangwang\nDailing\nWuyiling\nHongxing\nShangganling\nYouhao\nJiayin\nNancha\nTieli",
    "Jiamusi\nXiangyang\nQianjin\nDongfeng\nJiaoqu\nHuanan\nHuachuan\nTangyuan\nTongjiang\nFujin\nFuyuan",
    "Qitaihe\nXinxing\nTaoshan\nQiezihuo\nBoli",
    "Mudanjiang\nDong'an\nYangming\nAimin\nXi'an\nLinkou\nSuifenhe\nHailin\nNing'an\nMuleng\nDongning",
    "Heihe\nAihui\nNenjiang\nXunke\nSunwu\nBeian\nWudalianchi",
    "Suihua\nBeilin\nWangkui\nLanxi\nQinggang\nQing'an\nMingshui\nSuiling\nAnda\nZhaodong\nHailun",
    "Daxing'anling\nMohe\nHuma\nTahe\nJiagedaqi\nXinlin\nHuzhong",
    // Shanghai
    "Shanghai\nXuhui\nMinhang\nBaoshan\nJiading\nPudong New Area\nJinshan\nSongjiang\nQingpu\nFengxian\nChongming",
    "Huangpu",
    "Changning",
    "Jing'an",
    "Putuo",
    "Hongkou",
    "Yangpu",
    // Jiangsu
    "Nanjing\nXuanwu\nQinhuai\nJianye\nGulou\nPukou\nQixia\nYuhuatai\nJiangning\nLuhe\nLishui\nGaochun",
    "Wuxi\nXishan\nHuishan\nBinhu\nLiangxi\nXinwu\nJiangyin\nYixing",
    "Xuzhou\nGulou\nYunlong\nJiawang\nQuanshan\nTongshan\nFeng County\nPei County\nSuining\nXinyi\nPizhou",
    "Changzhou\nTianning\nZhonglou\nXinbei\nWujin\nLiyang\nJintan",
    "Suzhou\nHuqiu\nWuzhong\nXiangcheng\nGusu\nWujiang\nChangshu\nZhangjiagang\nKunshan\nTaicang",
    "Nantong\nChongchuan\nGangzha\nTongzhou\nRudong\nQidong\nRugao\nHaimen\nHaian",
    "Lianyungang\nLianyun\nHaizhou\nGanyu\nDonghai\nGuanyun\nGuannan",
    "Huai'an\nHuai'an District\nHuaiyin District\nQingjiangpu\nHongze\nLianshui\nXuyi\nJinhu",
    "Yancheng\nTinghu\nYandu\nDafeng\nXiangshui\nBinhai\nFuning\nSheyang\nJianhu\nDongtai",
    "Yangzhou\nGuangling\nHanjiang\nJiangdu\nBaoying\nYizheng\nGaoyou",
    "Zhenjiang\nJingkou\nRunzhou\nDantu\nDanyang\nYangzhong\nJurong",
    "Taizhou\nHailing\nGaogang\nJiangyan\nXinghua\nJingjiang\nTaixing",
    "Suqian\nSucheng\nSuyu\nShuyang\nSiyang\nSihong",
    // Zhejiang
    "Hangzhou\nShangcheng\nXiacheng\nJianggan\nGongshu\nXihu\nBinjiang\nXiaoshan\nYuhang\nTonglu\nChun'an\nJiande\nFuyang\nLin'an",
    "Ningbo\nHaishu\nJiangbei\nBeilun\nZhenhai\nYinzhou\nFenghua\nXiangshan\nNinghai\nYuyao\nCixi",
    "Wenzhou\nLucheng\nLongwan\nOuhai\nDongtou\nYongjia\nPingyang\nCangnan\nWencheng\nTaishun\nRuian\nYueqing",
    "Jiaxing\nNanhu\nXiuzhou\nJiashan\nHaiyan\nHaining\nPinghu\nTongxiang",
    "Huzhou\nWuxing\nNanxun\nDeqing\nChangxing\nAnji",
    "Shaoxing\nYuecheng\nKeqiao\nShangyu\nXinchang\nZhuji\nShengzhou",
    "Jinhua\nWucheng\nJindong\nWuyi\nPujiang\nPan'an\nLanxi\nYiwu\nDongyang\nYongkang",
    "Quzhou\nKecheng\nQuxiang\nChangshan\nKaihua\nLongyou\nJiangshan",
    "Zhoushan\nDinghai\nPutuo\nDaishan\nShengsi",
    "Taizhou\nJiaojiang\nHuangyan\nLuqiao\nSanmen\nTiantai\nXianju\nWenling\nLinhai\nYuhuan",
    "Lishui\nLiandu\nQingtian\nJinyun\nSuichang\nSongyang\nYunhe\nQingyuan\nJingning\nLongquan",
    // Anhui
    "Hefei\nYaohai\nLuyang\nShushan\nBaohe\nChangfeng\nFeidong\nFeixi\nLujiang\nChaohu",
    "Wuhu\nJinghu\nYijiang\nJiujiang\nSanshan\nWuhu County\nFanchang\nNanling\nWuwei",
    "Bengbu\nLongzihu\nBangshan\nYuhui\nHuaishang\nHuaiyuan\nWuhe\nGuzhen",
    "Huainan\nDatong\nTianjia'an\nXiejiaji\nBagongshan\nPanji\nFengtai\nShouxian",
    "Ma'anshan\nHuashan\nYushan\nBowang\nDangtu\nHanshan\nHexian",
    "Huaibei\nDuji\nXiangshan\nLieshan\nSuixi",
    "Tongling\nTongguan\nYi'an\nJiaoqu\nZongyang",
    "Anqing\nYingjiang\nDaguan\nYixiu\nHuaining\nTaihu\nSusong\nWangjiang\nYuexi\nTongcheng\nQianshan",
    "Huangshan\nTunxi\nHuangshan District\nHuangshan Scenic Area (Guangmingding)\nHuizhou\nShe County\nXiuning\nYi County\nQimen",
    "Chuzhou\nLangya\nNanqiao\nLai'an\nQuanjiao\nDingyuan\nFengyang\nTianchang\nMingguang",
    "Fuyang\nYingzhou\nYingdong\nYingquan\nLinquan\nTaihe\nFunan\nYingshang\nJieshou",
    "Suzhou\nYongqiao\nDangshan\nXiao County\nLingbi\nSixian",
    "Lu'an\nJin'an\nYu'an\nYeji\nHuoqiu\nShucheng\nJinzhai\nHuoshan",
    "Bozhou\nQiaocheng\nGuoyang\nMengcheng\nLixin",
    "Chizhou\nGuichi\nDongzhi\nShitai\nQingyang\nJiuhua Mountain",
    "Xuancheng\nXuanzhou\nLangxi\nGuangde\nJing County\nJixi\nJingde\nNingguo",
    // Fujian
    "Fuzhou\nGulou\nTaijiang\nCangshan\nMawei\nJin'an\nChangle\nMinhou\nLianjiang\nLuoyuan\nMinqing\nYongtai\nPingtan\nFuqing",
    "Xiamen\nSiming\nHaicang\nHuli\nJimei\nTong'an\nXiang'an",
    "Putian\nChengxiang\nHanjiang\nLicheng\nXiuyu\nXianyou",
    "Sanming\nMeilie\nSanyuan\nMingxi\nQingliu\nNinghua\nDatian\nYouxi\nShaxian\nJiangle\nTaining\nJianning\nYong'an",
    "Quanzhou\nLicheng\nFengze\nLuojiang\nQuangang\nHui'an\nChongwu\nAnxi\nYongchun\nDehua\nJinmen\nShishi\nJinjiang\nNan'an",
    "Zhangzhou\nXiangcheng\nLongwen\nYunxiao\nZhangpu\nZhao'an\nChangtai\nDongshan\nNanjing\nPinghe\nHua'an\nLonghai",
    "Nanping\nYanping\nJianyang\nShunchang\nPucheng\nGuangze\nSongxi\nZhenghe\nShaowu\nWuyishan\nJian'ou",
    "Longyan\nXinluo\nYongding\nChangting\nShanghang\nWuping\nLiancheng\nZhangping",
    "Ningde\nJiaocheng\nXiapu\nGutian\nPingnan\nShouning\nZhouning\nZherong\nFu'an\nFuding",
    // Jiangxi
    "Nanchang\nDonghu\nXihu\nQingyunpu\nWanli\nQingshanhu\nXinjian\nNanchang County\nAnyi\nJinxian",
    "Jingdezhen\nChangjiang\nZhushan\nFuliang\nLe Ping",
    "Pingxiang\nAnyuan\nXiangdong\nLianhua\nShangli\nLuxi",
    "Jiujiang\nLianxi\nXunyang\nChaisang\nWuning\nXiushui\nYongxiu\nDe'an\nDuchang\nHukou\nPengze\nRuichang\nGongqingcheng\nLushan",
    "Xinyu\nYushui\nFenyi",
    "Yingtan\nYuehu\nYujiang\nGuixi",
    "Ganzhou\nZhanggong\nNankang\nGan County\nXinfeng\nDayu\nShangyou\nChongyi\nAnyuan\nLongnan\nDingnan\nQuannan\nNingdu\nYudu\nXingguo\nHuichang\nXunwu\nShicheng\nRuijin",
    "Ji'an\nJizhou\nQingyuan\nJi'an County\nJishui\nXiajiang\nXingan\nYongfeng\nTaihe\nSuichuan\nWan'an\nAnfu\nYongxin\nJinggangshan",
    "Yichun\nYuanzhou\nFengxin\nWanzai\nShanggao\nYifeng\nJing'an\nTonggu\nFengcheng\nZhangshu\nGao'an",
    "Fuzhou\nLinchuan\nDongxiang\nNancheng\nLichuan\nNanfeng\nChongren\nLe'an\nYihuang\nJinxi\nZixi\nGuangchang",
    "Shangrao\nXinzhou\nGuangfeng\nShangrao County\nYushan\nYanshan\nHengfeng\nYiyang\nYugan\nPoyang\nWannian\nWuyuan\nDexing",
    // Shandong
    "Jinan\nLixia\nShizhong\nHuaiyin\nTianqiao\nLicheng\nChangqing\nJiyang\nLaiwu\nPingyin\nShanghe\nZhangqiu",
    "Gangcheng\nLaicheng",
    "Qingdao\nShinan\nShibei\nHuangdao\nLaoshan\nLicang\nChengyang\nJiaozhou\nJimo\nPingdu\nLaixi",
    "Zibo\nZichuan\nZhangdian\nBoshan\nLinzi\nZhoucun\nHuantai\nGaoqing\nYiyuan",
    "Zaozhuang\nShizhong\nXuecheng\nYicheng\nTai'erzhuang\nShanting\nTengzhou",
    "Dongying\nHekou\nKenli\nLijin\nGuangrao",
    "Yantai\nZhifu\nFushan\nMouping\nLaishan\nChangdao\nLongkou\nLaiyang\nLaizhou\nPenglai\nZhaoyuan\nQixia\nHaiyang",
    "Weifang\nWeicheng\nHanting\nFangzi\nKuiwen\nLinqu\nChangle\nQingzhou\nZhucheng\nShouguang\nAnqiu\nGaomi\nChangyi",
    "Jining\nRencheng\nYanzhou\nWeishan\nYutai\nJinxiang\nJiaxiang\nWenshang\nSishui\nLiangshan\nQufu\nZoucheng",
    "Tai'an\nTaishan\nDaiyue\nNingyang\nDongping\nXintai\nFeicheng",
    "Weihai\nHuancui\nWendeng\nRongcheng\nRushan\nShidao\nChengshantou",
    "Rizhao\nDonggang\nLanshan\nWulian\nJu County",
    "Linyi\nLanshan\nLuozhuang\nHedong\nYinan\nTancheng\nYishui\nLanling\nFei County\nPingyi\nJunan\nMengyin\nLinshu",
    "Dezhou\nDecheng\nLingcheng\nNingjin\nQingyun\nLinyi\nQihe\nPingyuan\nXiajin\nWucheng\nLeling\nYucheng",
    "Liaocheng\nDongchangfu\nYanggu\nShen County\nChiping\nDong'e\nGuan County\nGaotang\nLinqing",
    "Binzhou\nBincheng\nZhanhua\nHuimin\nYangxin\nWudi\nBoxing\nZouping",
    "Heze\nMudan\nDingtao\nCao County\nShan County\nChengwu\nJuye\nYuncheng\nJuancheng\nDongming",
    // Henan
    "Zhengzhou\nZhongyuan\nErqi\nGuancheng\nJinshui\nShangjie\nHuiji\nZhongmu\nGongyi\nXingyang\nXinmi\nXinzheng\nDengfeng",
    "Kaifeng\nLongting\nShunhe\nGulou\nYuwangtai\nXiangfu\nQi County\nTongxu\nWeishi\nLankao",
    "Luoyang\nLaocheng\nXigong\nChanhe\nJianxi\nJili\nLuolong\nMengjin\nXin'an\nLuanchuan\nSong County\nRuyang\nYiyang\nLuoning\nYichuan\nYanshi",
    "Pingdingshan\nXinhua\nWeidong\nShilong\nZhanhe\nBaofeng\nYe County\nLushan\nJia County\nWugang\nRuzhou",
    "Anyang\nWenfeng\nBeiguan\nYindu\nLong'an\nTangyin\nHua County\nNeihuang\nLinzhou",
    "Hebi\nHeshan\nShancheng\nQibin\nXun County\nQi County",
    "Xinxiang\nHongqi\nWeibin\nFengquan\nMuye\nHuojia\nYuanyang\nYanjin\nFengqiu\nChangyuan\nWeihui\nHuixian",
    "Jiaozuo\nJiefang\nZhongzhan\nMacun\nShanyang\nXiuwu\nBo'ai\nWuzhi\nWen County\nQinyang\nMengzhou",
    "Puyang\nHualong\nQingfeng\nNanle\nFan County\nTaiqian",
    "Xuchang\nWeidu\nJian'an\nYanling\nXiangcheng\nYuzhou\nChangge",
    "Luohe\nYuanhui\nYancheng\nZhaoling\nWuyang\nLinying",
    "Sanmenxia\nHubin\nShanzhou\nMianchi\nLushi\nYima\nLingbao",
    "Nanyang\nWancheng\nWolong\nNanzhao\nFangcheng\nXixia\nZhenping\nNeixiang\nXichuan\nSheqi\nTanghe\nXinye\nTongbai\nDengzhou",
    "Shangqiu\nLiangyuan\nSuiyang\nMinquan\nSui County\nNingling\nZhecheng\nYucheng\nXiayi\nYongcheng",
    "Xinyang\nShihe\nPingqiao\nLuoshan\nGuangshan\nXin County\nShangcheng\nGushi\nHuangchuan\nHuaibin\nXixian",
    "Zhoukou\nChuanhui\nFugou\nXihua\nShangshui\nShenqiu\nDancheng\nHuaiyang\nTaikang\nLuyi\nXiangcheng",
    "Zhumadian\nYicheng\nXiping\nShangcai\nPingyu\nZhengyang\nQueshan\nBiyang\nRunan\nSuiping\nXincai",
    "Jiyuan",
    // Hubei
    "Wuhan\nJiang'an\nJianghan\nQiaokou\nHanyang\nWuchang\nQingshan\nHongshan\nDongxihu\nHannan\nCaidian\nJiangxia\nHuangpi\nXinzhou",
    "Huangshi\nHuangshigang\nXisaishan\nXialu\nTieshan\nYangxin\nDaye",
    "Shiyan\nMaojian\nZhangwan\nYunyang\nYunxi\nZhushan\nZhuxi\nFang County\nDanjiangkou",
    "Yichang\nXiling\nWujiagang\nDianjun\nXiaoting\nYiling\nThree Gorges\nYuan'an\nXingshan\nZigui\nChangyang\nWufeng\nYidu\nDangyang\nZhijiang",
    "Xiangyang\nXiangcheng\nFancheng\nXiangzhou\nNanzhang\nGucheng\nBaokang\nLaohekou\nZaoyang\nYicheng",
    "Ezhou\nLiangzihu\nHuarong\nEcheng",
    "Jingmen\nDongbao\nDuodao\nShayang\nZhongxiang\nJingshan",
    "Xiaogan\nXiaonan\nXiaochang\nDawu\nYunmeng\nYingcheng\nAnlu\nHanchuan",
    "Jingzhou\nShashi\nGong'an\nJianli\nJiangling\nShishou\nHonghu\nSongzi",
    "Huanggang\nHuangzhou\nTuanfeng\nHong'an\nLuotian\nYingshan\nXishui\nQichun\nHuangmei\nMacheng\nWuxue",
    "Xianning\nXian'an\nJiayu\nTongcheng\nChongyang\nTongshan\nChibi",
    "Suizhou\nZengdu\nSuizhou County\nGuangshui",
    "Enshi\nLichuan\nJianshi\nBadong\nXuan'en\nXianfeng\nLaifeng\nHefeng",
    "Xiantao",
    "Qianjiang",
    "Tianmen",
    "Shennongjia",
    // Hunan
    "Changsha\nFurong\nTianxin\nYuelu\nKaifu\nYuhua\nWangcheng\nChangsha County\nLiuyang\nNingxiang\nXiangjiang New Area",
    "Zhuzhou\nHetang\nLusong\nShifeng\nTianyuan\nYou County\nChaling\nYanling\nLiling",
    "Xiangtan\nYuhu\nYuetang\nXiangxiang\nShaoshan",
    "Hengyang\nZhuhui\nYanfeng\nShigu\nZhengxiang\nNanyue\nHengyang County\nHengnan\nHengshan\nHengdong\nQidong\nLeiyang\nChangning",
    "Shaoyang\nShuangqing\nDaxiang\nBeita\nShaodong\nXinshao\nShaoyang County\nLonghui\nDongkou\nSuining\nXinning\nChengbu\nWugang",
    "Yueyang\nYueyanglou District\nYunxi\nJunshan\nHuarong\nXiangyin\nPingjiang\nMiluo\nLinxiang",
    "Changde\nWuling\nDingcheng\nAnxiang\nHanshou\nLi County\nLinli\nTaoyuan\nShimen\nJinshi",
    "Zhangjiajie\nYongding\nWulingyuan\nCili\nSangzhi",
    "Yiyang\nZiyang\nHeshan District\nNan County\nTaojiang\nAnhua\nYuanjiang",
    "Chenzhou\nBeihu\nSuxian\nGuiyang\nYizhang\nYongxing\nJiahe\nLinwu\nRucheng\nGuidong\nAnren\nZixing",
    "Yongzhou\nLingling\nLengshuijiang\nQiyang\nDong'an\nShuangpai\nDao County\nJiangyong\nNingyuan\nLanshan\nXintian\nJianghua",
    "Huaihua\nHecheng\nZhongfang\nYuanling\nChenxi\nXupu\nHuitong\nMayang\nXinhuang\nZhijiang\nJingzhou\nTongdao\nHongjiang",
    "Loudi\nLouxing\nShuangfeng\nXinhua\nLengshuijiang\nLianyuan",
    "Xiangxi\nJishou\nLuxi\nFenghuang\nHuayuan\nBaojing\nGuzhang\nYongshun\nLongshan",
    // Guangdong
    "Guangzhou\nLiwan\nYuexiu\nHaizhu\nTianhe\nBaiyun\nHuangpu\nPanyu\nHuadu\nNansha\nConghua\nZengcheng",
    "Shaoguan\nWujiang\nZhenjiang\nQujiang\nShixing\nRenhua\nWengyuan\nRuyuan\nXinfeng\nLechang\nNanxiong",
    "Shenzhen\nLuohu\nFutian\nNanshan\nBao'an\nLonggang\nYantian\nLonghua\nPingshan\nGuangming",
    "Zhuhai\nXiangzhou\nDoumen\nJinwan",
    "Shantou\nLonghu\nJinping\nHaojiang\nChaoyang\nChaonan\nChenghai\nNan'ao",
    "Foshan\nChancheng\nNanhai\nShunde\nSanshui\nGaoming",
    "Jiangmen\nPengjiang\nJianghai\nXinhui\nTaishan\nKaiping\nHeshan\nEnping",
    "Zhanjiang\nChikan\nXiashan\nPotou\nMazhang\nSuixi\nXuwen\nLianjiang\nLeizhou\nWuchuan",
    "Maoming\nMaonan\nDianbai\nGaozhou\nHuazhou\nXinyi",
    "Zhaoqing\nDuanzhou\nDinghu\nGaoyao\nGuangning\nHuaiji\nFengkai\nDeqing\nSihui",
    "Huizhou\nHuicheng\nHuiyang\nBoluo\nHuidong\nLongmen",
    "Meizhou\nMeijiang\nMeixian\nDabu\nFengshun\nWuhua\nPingyuan\nJiaoling\nXingning",
    "Shanwei\nHaifeng\nLuhe\nLufeng",
    "Heyuan\nYuancheng\nZijin\nLongchuan\nLianping\nHeping\nDongyuan",
    "Yangjiang\nJiangcheng\nYangdong\nYangxi\nYangchun",
    "Qingyuan\nQingcheng\nQingxin\nFogang\nYangshan\nLianshan\nLiannan\nYingde\nLianzhou",
    "Dongguan",
    "Zhongshan",
    "Chaozhou\nXiangqiao\nChao'an\nRaoping",
    "Jieyang\nRongcheng\nJiedong\nJiexi\nHuilai\nPuning",
    "Yunfu\nYuncheng\nYun'an\nXinxing\nYunan\nLuoding",
    // Guangxi
    "Nanning\nXingning\nQingxiu\nJiangnan\nXixiangtang\nLiangqing\nYongning\nWuming\nLong'an\nMashan\nShanglin\nBinyang\nHengzhou",
    "Liuzhou\nChengzhong\nYufeng\nLiunan\nLiubei\nLiujiang\nLiucheng\nLuzhai\nRong'an\nRongshui\nSanjiang",
    "Guilin\nXiufeng\nDiecai\nXiangshan\nQixing\nYanshan\nLingui\nYangshuo\nLingchuan\nQuanzhou\nXing'an\nYongfu\nGuanyang\nLongsheng\nZiyuan\nPingle\nGongcheng\nLipu",
    "Wuzhou\nWanxiu\nChangzhou\nLongxu\nCangwu\nTeng County\nMengshan\nCenxi",
    "Beihai\nHaicheng\nWeizhou Island\nYinhai\nTieshangang\nHepu",
    "Fangchenggang\nGangkou\nFangcheng\nShangsi\nDongxing",
    "Qinzhou\nQinnan\nQinbei\nLingshan\nPubei",
    "Guigang\nGangbei\nGangnan\nQintang\nPingnan\nGuiping",
    "Yulin\nYuzhou\nFumian\nRong County\nLuchuan\nBobai\nXingye\nBeiliu",
    "Baise\nYoujiang\nTianyang\nTiandong\nPingguo\nDebao\nNapo\nLingyun\nLeye\nTianlin\nXilin\nLonglin\nJingxi",
    "Hezhou\nBabbu\nPinggui\nZhaoping\nZhongshan\nFuchuan",
    "Hechi\nJinchengjiang\nYizhou\nNandan\nTian'e\nFengshan\nDonglan\nLuocheng\nHuanjiang\nBama\nDu'an\nDahua",
    "Laibin\nXingbin\nXincheng\nXiangzhou\nWuxuan\nJinxiu\nHeshan",
    "Chongzuo\nJiangzhou\nFusui\nNingming\nLongzhou\nDaxin\nTiandeng\nPingxiang",
    // Hainan
    "Haikou\nXiuying\nLonghua\nQiongshan\nMeilan",
    "Sanya\nHaitang\nJiyang\nTianya\nYazhou",
    "Sansha\nXisha\nNansha\nZhongsha",
    "Danzhou",
    "Wuzhishan",
    "Qionghai",
    "Wenchang",
    "Wanning",
    "Dongfang",
    "Ding'an",
    "Tunchang",
    "Chengmai",
    "Lingao",
    "Baisha",
    "Changjiang",
    "Ledong",
    "Lingshui",
    "Baoting",
    "Qiongzhong",
    // Chongqing
    "Chongqing\nWanzhou\nFuling\nBeibei\nQijiang\nDazu\nYubei\nBanan\nQianjiang\nChangshou\nJiangjin\nHechuan\nYongchuan\nNanchuan\nBishan\nTongliang\nTongnan\nRongchang\nLiangping\nWulong\nChengkou\nFengdu\nDianjiang\nZhong County\nYunyang\nFengjie\nWushan\nWuxi\nShizhu\nXiushan\nYouyang\nPengshui",
    "Yuzhong",
    "Dadukou",
    "Jiangbei",
    "Shapingba",
    "Jiulongpo",
    "Nan'an",
    "Kaizhou",
    // Sichuan
    "Chengdu\nJinjiang\nQingyang\nJinniu\nWuhou\nChenghua\nLongquanyi\nQingbaijiang\nXindu\nWenjiang\nShuangliu\nPidu\nJintang\nDayi\nPujiang\nXinjin\nDujiangyan\nPengzhou\nQionglai\nChongzhou\nJianyang",
    "Zigong\nZiliujing\nGongjing\nDaan\nYantan\nRong County\nFushun",
    "Panzhihua\nDongqu\nXiqu\nRenhe\nMiyi\nYanbian",
    "Luzhou\nJiangyang\nNaxi\nLongmatan\nLu County\nHejiang\nXuyong\nGulin",
    "Deyang\nJingyang\nLuojiang\nZhongjiang\nGuanghan\nShifang\nMianzhu",
    "Mianyang\nFucheng\nYouxian\nAnzhou\nSantai\nYanting\nZitong\nBeichuan\nPingwu\nJiangyou",
    "Guangyuan\nLizhou\nZhaohua\nChaotian\nWangcang\nQingchuan\nJiange\nCangxi",
    "Suining\nChuanshan\nAnju\nPengxi\nShehong\nDaying",
    "Neijiang\nShizhong\nDongxing\nWeiyuan\nZizhong\nLongchang",
    "Leshan\nShizhong\nShawan\nWutongqiao\nJinkouhe\nQianwei\nJingyan\nJiajiang\nMuchuan\nEbian\nMabian\nEmeishan",
    "Nanchong\nShunqing\nGaoping\nJialing\nNanbu\nYingshan\nPeng'an\nYilong\nXichong\nLangzhong",
    "Meishan\nDongpo\nPengshan\nRenshou\nHongya\nDanling\nQingshen",
    "Yibin\nCuiping\nNanxi\nYibin County\nJiang'an\nChangning\nGaoxian\nGongxian\nJunlian\nXingwen\nPingshan",
    "Guang'an\nQianfeng\nYuechi\nWusheng\nLinshui\nHuaying",
    "Dazhou\nTongchuan\nDachuan\nXuanhan\nKaijiang\nDazhu\nQu County\nWanyuan",
    "Ya'an\nYucheng\nMingshan\nYingjing\nHanyuan\nShimian\nTianquan\nLushan\nBaoxing",
    "Bazhong\nBazhou\nEnyang\nTongjiang\nNanjiang\nPingchang",
    "Ziyang\nYanjiang\nAnyue\nLezhi",
    "Aba\nMa'erkang\nWenchuan\nLixian\nMaoxian\nSongpan\nJiuzhaigou\nJinchuan\nXiaojin\nHeishui\nRangtang\nRuoergai\nHongyuan",
    "Ganzi\nKangding\nLuding\nDanba\nJiulong\nYajiang\nDaofu\nLuhuo\nXinlong\nDege\nBaiyu\nShiqu\nSeda\nLitang\nBatang\nXiangcheng\nDaocheng\nDerong",
    "Liangshan\nXichang\nMuli\nYanyuan\nDechang\nHuili\nHuidong\nNingnan\nPuge\nButuo\nJinyang\nZhaojue\nXide\nMianning\nYuexi\nGanluo\nMeigu\nLeibo",
    // Guizhou
    "Guiyang\nNanming\nYunyan\nHuaxi\nWudang\nBaiyun\nGuanshanhu\nKaiyang\nXifeng\nXiuwen\nQingzhen",
    "Liupanshui\nZhongshan\nLiuzhi\nShuicheng\nPanzhou",
    "Zunyi\nHonghuagang\nHuichuan\nBozhou\nTongzi\nSuiyang\nZhengan\nDaozhen\nWuchuan\nFenggang\nMeitan\nYuqing\nXishui\nChishui\nRenhuai",
    "Anshun\nXixiu\nPingba\nPuding\nZhenning\nGuanling\nZiyun",
    "Bijie\nQixingguan\nDafang\nQianxi\nJinsha\nZhijin\nNayong\nWeining\nHezhang",
    "Tongren\nBijiang\nWanshan\nJiangkou\nYuping\nShiqian\nSinan\nYinjiang\nDejiang\nYanhe\nSongtao",
    "South Guizhou\nXingyi\nXingren\nPu'an\nQinglong\nZhenfeng\nWangmo\nCeheng\nAnlong",
    "Southeast Guizhou\nKaili\nHuangping\nShibing\nSansui\nZhenyuan\nCengong\nTianzhu\nJinping\nJianhe\nTaijiang\nLiping\nRongjiang\nCongjiang\nLeishan\nMajiang\nDanzhai",
    "Southwest Guizhou\nDuyun\nFuquan\nLibo\nGuiding\nWeng'an\nDushan\nPingtang\nLuodian\nChangshun\nLongli\nHuishui\nSandu",
    // Yunnan
    "Kunming\nWuhua\nPanlong\nGuandu\nXishan\nDongchuan\nChenggong\nJinning\nFumin\nYiliang\nShilin\nSongming\nLuquan\nXundian\nAnning",
    "Qujing\nQilin\nZhanyi\nMalong\nLuliang\nShizong\nLuoping\nFuyuan\nHuize\nXuanwei",
    "Yuxi\nHongta\nJiangchuan\nChengjiang\nTonghai\nHuaning\nYimen\nEshan\nXinping\nYuanjiang",
    "Baoshan\nLongyang\nShidian\nLongling\nChangning\nTengchong",
    "Zhaotong\nZhaoyang\nLudian\nQiaojia\nYanjin\nDaguan\nYongshan\nSuijiang\nZhenxiong\nYiliang\nWeixin\nShuifu",
    "Lijiang\nGucheng\nYulong\nYongsheng\nHuaping\nNinglang",
    "Puer\nSimao\nNing'er\nMojiang\nJingdong\nJinggu\nZhenyuan\nJiangcheng\nMenglian\nLancang\nXimeng",
    "Lincang\nLinxiang\nFengqing\nYunxian\nYongde\nZhenkang\nShuangjiang\nGengma\nCangyuan",
    "Chuxiong\nChuxiong\nShuangbai\nMouding\nNanhua\nYao'an\nDayao\nYongren\nYuanmou\nWuding\nLufeng",
    "Honghe\nGejiu\nKaiyuan\nMengzi\nMile\nPingbian\nJianshui\nShiping\nLuxi\nYuanyang\nJinping\nLuchun\nHekou",
    "Wenshan\nWenshan\nYanshan\nXichou\nMalipo\nMaguan\nQiubei\nGuangnan\nFuning",
    "Xishuangbanna\nJinghong\nMenghai\nMengla",
    "Dali\nDali\nYangbi\nXiangyun\nBinchuan\nMidu\nNanjian\nWeishan\nYongping\nYunlong\nEryuan\nJianchuan\nHeqing",
    "Dehong\nRuili\nMangshi\nLianghe\nYingjiang\nLongchuan",
    "Nujiang\nLushui\nFugong\nGongshan\nLanping",
    "Diqing\nShangri-La\nDeqin\nWeixi",
    // Tibet
    "Lhasa\nChengguan\nDuilongdeqing\nDagzê\nLhunzhub\nDamxung\nNyêmo\nQushui\nMaizhokunggar",
    "Xigazê\nSamzhubzê\nNamling\nGyangzê\nTingri\nSa'gya\nLhatse\nNgamring\nXaitongmoin\nBainang\nRinbung\nKangmar\nDinggyê\nZhongba\nYadong\nPagri\nGyirong\nNyalam\nSaga\nGamba",
    "Qamdo\nKaruo\nJiangda\nGonjo\nChamdo\nDêngqên\nZayü\nBaxoi\nLeft\nMarkam\nLhorong\nBanbar",
    "Nyingchi\nBayi\nGongbo'gyamda\nMainling\nMedog\nBomi\nZayü\nLangxian",
    "Shannan\nNêdong\nZedang\nZhaxoi\nGonggar\nSangri\nQonggyai\nQusum\nComai\nLhozhag\nGyacha\nLhunze\nCona\nNagarzê",
    "Nagqu\nSeni\nJiali\nBiru\nNyainrong\nAmdo\nXainza\nSog\nBaingoin\nBaxoi\nNyima\nShuanghu",
    "Ngari\nPurang\nZanda\nGar\nShiquanhe\nRutog\nGê'gyai\nGêrzê\nCoqên",
    // Shaanxi
    "Xi'an\nXincheng\nBeilin\nLianhu\nBaqiao\nWeiyang\nYanta\nYanliang\nLintong\nChang'an\nGaoling\nHuyi\nLantian\nZhouzhi",
    "Tongchuan\nWangyi\nYintai\nYaozhou\nYijun",
    "Baoji\nWeibin\nJintai\nChencang\nFengxiang\nQishan\nFufeng\nMeixian\nLongxian\nQianyang\nLinyou\nFengxian\nTaibai",
    "Yangling",
    "Xianyang\nQindu\nYangling\nWeicheng\nSanyuan\nJingyang\nQian County\nLiquan\nYongshou\nChangwu\nXunyi\nChunhua\nWugong\nXingping\nBin County",
    "Weinan\nLinwei\nHuazhou\nTongguan\nDali\nHeyang\nChengcheng\nPucheng\nBaishui\nFuping\nHancheng\nHuayin",
    "Yan'an\nBaota\nAnsai\nYanchang\nYanchuan\nZichang\nZhidan\nWuqi\nGanquan\nFu County\nLuochuan\nYichuan\nHuanglong\nHuangling",
    "Hanzhong\nHantai\nNanzheng\nChenggu\nYang County\nXixiang\nMian County\nNingqiang\nLueyang\nZhenba\nLiuba\nFoping",
    "Yulin\nYuyang\nHengshan\nFugu\nJingbian\nDingbian\nSuide\nMizhi\nJia County\nWubu\nQingjian\nZizhou\nShenmu",
    "Ankang\nHanbin\nHanyin\nShiquan\nNingshan\nZiyang\nLangao\nPingli\nZhenping\nXunyang\nBaihe",
    "Shangluo\nShangzhou\nLuonan\nDanfeng\nShangnan\nShanyang\nZhen'an\nZhashui",
    // Gansu
    "Lanzhou\nChengguan\nQilihe\nXigu\nAnning\nHonggu\nYongdeng\nGaolan\nYuzhong",
    "Jiayuguan",
    "Jinchang\nJinchuan\nYongchang",
    "Baiyin\nPingchuan\nJingyuan\nHuining\nJingtai",
    "Tianshui\nQinzhou\nMaiji\nQingshui\nQin'an\nGangu\nWushan\nZhangjiachuan",
    "Wuwei\nLiangzhou\nMinqin\nGulang\nTianzhu",
    "Zhangye\nGanzhou\nSunan\nMinle\nLinze\nGaotai\nShandan",
    "Pingliang\nKongtong\nJingchuan\nLingtai\nChongxin\nZhuanglang\nJingning\nHuating",
    "Jiuquan\nSuzhou\nJinta\nGuazhou\nSubei\nAkesai\nYumen\nDunhuang",
    "Qingyang\nXifeng\nQingcheng\nHuan County\nHuachi\nHeshui\nZhengning\nNing County\nZhenyuan",
    "Dingxi\nAnding\nTongwei\nLongxi\nWeiyuan\nLintao\nZhang County\nMin County",
    "Longnan\nWudu\nCheng County\nWen County\nTanchang\nKang County\nXihe\nLi County\nHui County\nLiangdang",
    "Linxia\nKangle\nYongjing\nGuanghe\nHezheng\nDongxiang\nJishishan",
    "Gannan\nHezuo\nLintan\nZhuoni\nZhouqu\nDiebu\nMaqu\nLuqu\nXiahe",
    // Qinghai
    "Xining\nChengdong\nChengzhong\nChengxi\nChengbei\nDatong\nHuangzhong\nHuangyuan",
    "Haidong\nLedong\nPing'an\nMinhe\nHuzhu\nHualong\nXunhua",
    "Haibei\nMenyuan\nQilian\nHaiyan\nGangcha",
    "Huangnan\nTongren\nJianzha\nZeku\nHenan",
    "Hainan\nGonghe\nTongde\nGuide\nXinghai\nGuinan",
    "Golog\nMaqin\nBanma\nGande\nDari\nJigzhi\nMaduo",
    "Yushu\nZadoi\nChindu\nZhidoi\nNangqian\nQumarlêb",
    "Haixi\nGolmud\nDelingha\nMangya\nLenghu\nWulan\nDulan\nTianjun\nDa Qaidam",
    // Ningxia
    "Yinchuan\nXingqing\nXixia\nJinfeng\nYongning\nHelan\nLingwu",
    "Shizuishan\nDawukou\nHuinong\nPingluo\nTaole",
    "Wuzhong\nLitong\nHongsibu\nYanchi\nTongxin\nQingtongxia",
    "Guyuan\nYuanzhou\nXiji\nLongde\nJingyuan\nPengyang",
    "Zhongwei\nShapotou\nZhongning\nHaiyuan",
    // Xinjiang
    "Urumqi\nTianshan\nSaybagh\nXinshi\nShuimogou\nToutunhe\nDabancheng\nMidong\nUrumqi County\nXiaoquzi\nXibaiyanggou\nTianchi",
    "Karamay\nDushanzi\nBaijiantan\nUrho",
    "Turpan\nGaochang\nShanshan\nTuokexun",
    "Hami\nYiwu\nBarkol\nYiwu",
    "Changji\nFukang\nHutubi\nManas\nQitai\nJimusaer\nMulei\nCaijiahu",
    "Bortala\nBole\nAlashankou\nJinghe\nWenquan",
    "Bayingolin\nKorla\nLuntai\nYuli\nRuoqiang\nTieganlike\nQiemo\nTazhong\nYanqi\nHejing\nBaluntai\nBayanbulak\nHoxud\nBohu",
    "Aksu\nWensu\nKuqa\nShaya\nXinhe\nBaicheng\nWushi\nAwat\nKalpin",
    "Kizilsu\nArtux\nAkto\nAkqi\nWuqia",
    "Kashgar\nShufu\nShule\nYengisar\nZepu\nShache\nYecheng\nMakit\nYopurga\nJiashi\nBachu\nTaxkorgan",
    "Hotan\nMoyu\nPishan\nLop\nQira\nYutian\nMinfeng",
    "Ili\nYining\nKuitun\nKhorgos\nYining County\n察布查尔\nHuocheng\nGongliu\nXinyuan\nZhaosu\nTekes\nNilka",
    "Tacheng\nWusu\nEmin\nShawan\nToli\nYumin\nHoboksar",
    "Altay\nBurqin\nFuyun\nFuhai\nHabahe\nQinghe\nJimunai",
    "Shihezi\nPaotai\nMosowan",
    "Alar",
    "Tumushuke",
    "Wujiaqu",
    "Beitun",
    "Tiemenguan",
    "Shuanghe",
    "Kokdala",
    "Kunyu",
    // Taiwan
    "Taipei\nYilan\nTaoyuan\nHsinchu",
    "Kaohsiung\nTainan\nChiayi\nPingtung\nTaitung",
    "Taichung\nMiaoli\nChanghua\nNantou\nYunlin\nHualien",
    // Hong Kong
    "Hong Kong\nKowloon\nNew Territories",
    // Macao
    "Macao\nTaipa Island\nColoane Island",
};
char *city_code[][25] = {
    // 北京0
    {"110000", "110101", "110105", "110106", "110107", "110108", "110109", "110111", "110112", "110113", "110114", "110115", "110116", "110117", "110118", "110119"},
    {"110102"},
    // 天津1
    {"120000", "120110", "120111", "120112", "120113", "120114", "120115", "120116", "120117", "120118", "120119"},
    {"120101"},
    {"120102"},
    {"120103"},
    {"120104"},
    {"120105"},
    {"120106"},
    // 河北2
    {"130100", "130102", "130104", "130105", "130107", "130108", "130109", "130110", "130111", "130121", "130123", "130125", "130126", "130127", "130128", "130129", "130130", "130131", "130132", "130133", "130181", "130183", "130184"},
    {"130200", "130202", "130203", "130204", "130205", "130207", "130208", "130209", "130224", "130225", "130227", "130229", "130281", "130283", "130284"},
    {"130300", "130302", "130303", "130304", "130306", "130321", "130322", "130324"},
    {"130400", "130402", "130403", "130404", "130406", "130407", "130408", "130423", "130424", "130425", "130426", "130427", "130430", "130431", "130432", "130433", "130434", "130435", "130481"},
    {"130500", "130502", "130503", "130522", "130523", "130524", "130525", "130526", "130527", "130528", "130529", "130530", "130531", "130532", "130533", "130534", "130535", "130581", "130582"},
    {"130600", "130602", "130606", "130607", "130608", "130609", "130623", "130624", "130626", "130627", "130628", "130629", "130630", "130631", "130632", "130633", "130634", "130635", "130636", "130637", "130638", "130681", "130682", "130683", "130684"},
    {"130700", "130702", "130703", "130705", "130706", "130708", "130709", "130722", "130723", "130724", "130725", "130726", "130727", "130728", "130730", "130731", "130732"},
    {"130800", "130802", "130803", "130804", "130821", "130822", "130824", "130825", "130826", "130827", "130828", "130881"},
    {"130900", "130902", "130903", "130921", "130922", "130923", "130924", "130925", "130926", "130927", "130928", "130929", "130930", "130981", "130982", "130983", "130984"},
    {"131000", "131002", "131003", "131022", "131023", "131024", "131025", "131026", "131028", "131081", "131082"},
    {"131100", "131102", "131103", "131121", "131122", "131123", "131124", "131125", "131126", "131127", "131128", "131182"},
    // 山西3
    {"140100", "140105", "140106", "140107", "140108", "140109", "140110", "140121", "140122", "140123", "140181"},
    {"140200", "140203", "140211", "140212", "140215", "140221", "140222", "140223", "140224", "140225", "140226"},
    {"140300", "140303", "140311", "140321", "140322"},
    {"140400", "140405", "140406", "140411", "140423", "140425", "140426", "140427", "140428", "140429", "140430", "140431"},
    {"140500", "140521", "140522", "140524", "140525", "140581"},
    {"140600", "140602", "140603", "140621", "140622", "140623", "140681"},
    {"140700", "140702", "140721", "140722", "140723", "140724", "140725", "140726", "140727", "140728", "140729", "140781"},
    {"140800", "140802", "140821", "140822", "140823", "140824", "140825", "140826", "140827", "140828", "140829", "140830", "140881", "140882"},
    {"140900", "140902", "140921", "140922", "140923", "140924", "140925", "140926", "140927", "140928", "140929", "140930", "140931", "140932", "140971", "140981"},
    {"141000", "141002", "141021", "141022", "141023", "141024", "141025", "141026", "141027", "141028", "141029", "141030", "141031", "141032", "141033", "141034", "141081", "141082"},
    {"141100", "141102", "141121", "141122", "141123", "141124", "141125", "141126", "141127", "141128", "141129", "141130", "141181", "141182"},
    // 内蒙古4
    {"150100", "150102", "150103", "150104", "150105", "150121", "150122", "150123", "150124", "150125"},
    {"150200", "150202", "150203", "150204", "150205", "150206", "150207", "150221", "150222", "150223", "150223102", "150223103"},
    {"150300", "150302", "150303", "150304"},
    {"150400", "150402", "150403", "150404", "15040420", "150421", "150422", "150423", "150424", "150425", "150426", "150428", "150429", "15042910", "150430", "1504301072"},
    {"150500", "150502", "150521", "15052110", "150522", "150523", "150524", "150525", "15052510", "150526", "15052610", "150581"},
    {"150600", "150602", "150603", "150621", "150622", "150623", "150624", "150625", "15062520", "150626", "150626101", "1506261042", "150627"},
    {"150700", "150702", "150703", "150721", "150722", "1507221002", "150723", "15072310", "1507234150", "150724", "150725", "150726", "150727", "150781", "150782", "150783", "150784", "150785"},
    {"150800", "150802", "150821", "150822", "150822200205", "150823", "1508231050", "150824", "150825", "150826", "150999"},
    {"150900", "150902", "150921", "150922", "150923", "150924", "150925", "150926", "150927", "150928", "150929", "150981"},
    {"152200", "152201", "152202", "152221", "1522211010", "152222", "152223", "1522231030", "152224"},
    {"152500", "152501", "152502", "152522", "152523", "152524", "1525241010", "152525", "152526", "152527", "152528", "152529", "152530", "152531", "15257100"},
    {"152900", "152921", "1529211120", "1529212140", "152922", "1529221040", "152923", "1529232040", "15297110", "152999"},
    // 辽宁
    {"210100", "210102", "210103", "210104", "210105", "210106", "210111", "210112", "210113", "210114", "210115", "210123", "210124", "210181"},
    {"210200", "210202", "210203", "210204", "210211", "210212", "210213", "210214", "210224", "210281", "210283"},
    {"210300", "210302", "210303", "210304", "210311", "210321", "210323", "210381"},
    {"210400", "210402", "210403", "210404", "210411", "210422", "210423"},
    {"210500", "210502", "210503", "210504", "210505", "210521", "210522"},
    {"210600", "210602", "210603", "210604", "210624", "210681", "210682"},
    {"210700", "210702", "210703", "210711", "210726", "210727", "210781", "210782"},
    {"210800", "210802", "210803", "210804", "210811", "210881", "210882"},
    {"210900", "210902", "210903", "210904", "210905", "210911", "210922"},
    {"211000", "211002", "211003", "211004", "211005", "211011", "211021", "211081"},
    {"211100", "211102", "211103", "211104", "211122"},
    {"211200", "211202", "211204", "211223", "211224", "211281", "211282"},
    {"211300", "211302", "211303", "211322", "211324", "211381", "211382"},
    {"211400", "211402", "211403", "211404", "211421", "211422", "211481"},
    // 吉林
    {"220100", "220102", "220103", "220104", "220105", "220106", "220112", "220113", "220122", "220182", "220183", "220381"},
    {"220200", "220202", "220203", "220204", "220211", "220221", "220281", "220282", "220283", "220284"},
    {"220300", "220302", "220303", "220322", "220323", "220382"},
    {"220400", "220402", "220403", "220421", "220422"},
    {"220500", "220502", "220503", "220521", "220523", "220524", "220581", "220582"},
    {"220600", "220602", "220605", "220621", "22062110", "220622", "220623", "220681"},
    {"220700", "220702", "220721", "220722", "220723", "220781"},
    {"220800", "220802", "220821", "220822", "220881", "220882"},
    {"222400", "222401", "222402", "222403", "222404", "222405", "222406", "222424", "222426"},
    // 黑龙江
    {"230100", "230102", "230103", "230104", "230108", "230109", "230110", "230111", "230112", "230113", "230123", "230124", "230125", "230126", "230127", "230128", "230129", "230183", "230184"},
    {"230200", "230202", "230203", "230204", "230205", "230206", "230207", "230208", "230221", "230223", "230224", "230225", "230227", "230229", "230230", "230231", "230281"},
    {"230300", "230302", "230303", "230304", "230305", "230306", "230307", "230321", "230381", "230382"},
    {"230400", "230402", "230403", "230404", "230405", "230406", "230407", "230421", "230422"},
    {"230500", "230502", "230503", "230505", "230506", "230521", "230522", "230523", "230524"},
    {"230600", "230602", "230603", "230604", "230605", "230606", "230621", "230622", "230623", "230624"},
    {"230700", "230705", "230706", "230707", "230708", "230709", "230710", "230711", "230712", "230713", "230714", "230715", "230716", "230719", "230722", "230726", "230781"},
    {"230800", "230803", "230804", "230805", "230811", "230822", "230826", "230828", "230881", "230882", "230883"},
    {"230900", "230902", "230903", "230904", "230921"},
    {"231000", "231002", "231003", "231004", "231005", "231025", "231081", "231083", "231084", "231085", "231086"},
    {"231100", "231102", "231121", "231123", "231124", "231181", "231182"},
    {"231200", "231202", "231221", "231222", "231223", "231224", "231225", "231226", "231281", "231282", "231283"},
    {"232700", "232701", "232721", "232722", "232761", "232763", "232764"},
    // 上海
    {"310000", "310112", "310113", "310114", "310115", "310116", "310117", "310118", "310120", "310151"},
    {"310101"},
    {"310105"},
    {"310106"},
    {"310107"},
    {"310109"},
    {"310110"},
    // 江苏
    {"320100", "320102", "320104", "320105", "320106", "320111", "320113", "320114", "320115", "320116", "320117", "320118"},
    {"320200", "320205", "320206", "320211", "320213", "320214", "320281", "320282"},
    {"320300", "320302", "320303", "320305", "320311", "320312", "320321", "320322", "320324", "320381", "320382"},
    {"320400", "320402", "320404", "320411", "320412", "320481", "320482"},
    {"320500", "320505", "320506", "320507", "320508", "320509", "320581", "320582", "320583", "320585"},
    {"320600", "320602", "320611", "320612", "320623", "320681", "320682", "320684", "320685"},
    {"320700", "320703", "320706", "320707", "320722", "320723", "320724"},
    {"320800", "320803", "320804", "320812", "320813", "320826", "320830", "320831"},
    {"320900", "320902", "320903", "320904", "320921", "320922", "320923", "320924", "320925", "320981"},
    {"321000", "321002", "321003", "321012", "321023", "321081", "321084"},
    {"321100", "321102", "321111", "321112", "321181", "321182", "321183"},
    {"321200", "321202", "321203", "321204", "321281", "321282", "321283"},
    {"321300", "321302", "321311", "321322", "321323", "321324"},
    // 浙江
    {"330100", "330102", "330103", "330104", "330105", "330106", "330108", "330109", "330110", "330122", "330127", "330182", "330183", "330185"},
    {"330200", "330203", "330205", "330206", "330211", "330212", "330213", "330225", "330226", "330281", "330282"},
    {"330300", "330302", "330303", "330304", "330305", "330324", "330326", "330327", "330328", "330329", "330381", "330382"},
    {"330400", "330402", "330411", "330421", "330424", "330481", "330482", "330483"},
    {"330500", "330502", "330503", "330521", "330522", "330523"},
    {"330600", "330602", "330603", "330604", "330624", "330681", "330683"},
    {"330700", "330702", "330703", "330723", "330726", "330727", "330781", "330782", "330783", "330784"},
    {"330800", "330802", "330803", "330822", "330824", "330825", "330881"},
    {"330900", "330902", "330903", "330921", "330922"},
    {"331000", "331002", "331003", "331004", "331022", "331023", "331024", "331081", "331082", "331083"},
    {"331100", "331102", "331121", "331122", "331123", "331124", "331125", "331126", "331127", "331181"},
    // 安徽
    {"340100", "340102", "340103", "340104", "340111", "340121", "340122", "340123", "340124", "340181"},
    {"340200", "340202", "340203", "340207", "340208", "340221", "340222", "340223", "340225"},
    {"340300", "340302", "340303", "340304", "340311", "340321", "340322", "340323"},
    {"340400", "340402", "340403", "340404", "340405", "340406", "340421", "340422"},
    {"340500", "340503", "340504", "340506", "340521", "340522", "340523"},
    {"340600", "340602", "340603", "340604", "340621"},
    {"340700", "340705", "340706", "340711", "340722"},
    {"340800", "340802", "340803", "340811", "340822", "340825", "340826", "340827", "340828", "340881", "340882"},
    {"341000", "341002", "341003", "34100340", "341004", "341021", "341022", "341023", "341024"},
    {"341100", "341102", "341103", "341122", "341124", "341125", "341126", "341181", "341182"},
    {"341200", "341202", "341203", "341204", "341221", "341222", "341225", "341226", "341282"},
    {"341300", "341302", "341321", "341322", "341323", "341324"},
    {"341500", "341502", "341503", "341504", "341522", "341523", "341524", "341525"},
    {"341600", "341602", "341621", "341622", "341623"},
    {"341700", "341702", "341721", "341722", "341723", "341999"},
    {"341800", "341802", "341821", "341822", "341823", "341824", "341825", "341881"},
    // 福建
    {"350100", "350102", "350103", "350104", "350105", "350111", "350112", "350121", "350122", "350123", "350124", "350125", "350128", "350181"},
    {"350200", "350203", "350205", "350206", "350211", "350212", "350213"},
    {"350300", "350302", "350303", "350304", "350305", "350322"},
    {"350400", "350402", "350403", "350421", "350423", "350424", "350425", "350426", "350427", "350428", "350429", "350430", "350481"},
    {"350500", "350502", "350503", "350504", "350505", "350521", "3505211070", "350524", "350525", "350526", "350527", "350581", "350582", "350583"},
    {"350600", "350602", "350603", "350622", "350623", "350624", "350625", "350626", "350627", "350628", "350629", "350681"},
    {"350700", "350702", "350703", "350721", "350722", "350723", "350724", "350725", "350781", "350782", "350783"},
    {"350800", "350802", "350803", "350821", "350823", "350824", "350825", "350881"},
    {"350900", "350902", "350921", "350922", "350923", "350924", "350925", "350926", "350981", "350982"},
    // 江西
    {"360100", "360102", "360103", "360104", "360105", "360111", "360112", "360121", "360123", "360124"},
    {"360200", "360202", "360203", "360222", "360281"},
    {"360300", "360302", "360313", "360321", "360322", "360323"},
    {"360400", "360402", "360403", "360404", "360423", "360424", "360425", "360426", "360428", "360429", "360430", "360481", "360482", "360483"},
    {"360500", "360502", "360521"},
    {"360600", "360602", "360603", "360681"},
    {"360700", "360702", "360703", "360704", "360722", "360723", "360724", "360725", "360726", "360727", "360728", "360729", "360730", "360731", "360732", "360733", "360734", "360735", "360781"},
    {"360800", "360802", "360803", "360821", "360822", "360823", "360824", "360825", "360826", "360827", "360828", "360829", "360830", "360881"},
    {"360900", "360902", "360921", "360922", "360923", "360924", "360925", "360926", "360981", "360982", "360983"},
    {"361000", "361002", "361003", "361021", "361022", "361023", "361024", "361025", "361026", "361027", "361028", "361030"},
    {"361100", "361102", "361103", "361121", "361123", "361124", "361125", "361126", "361127", "361128", "361129", "361130", "361181"},
    // 山东
    {"370100", "370102", "370103", "370104", "370105", "370112", "370113", "370115", "370116", "370117", "370124", "370126", "370181"},
    {"370117", "371202"},
    {"370200", "370202", "370203", "370211", "370212", "370213", "370214", "370281", "370282", "370283", "370285"},
    {"370300", "370302", "370303", "370304", "370305", "370306", "370321", "370322", "370323"},
    {"370400", "370402", "370403", "370404", "370405", "370406", "370481"},
    {"370500", "370503", "370505", "370522", "370523"},
    {"370600", "370602", "370611", "370612", "370613", "370634", "370681", "370682", "370683", "370684", "370685", "370686", "370687"},
    {"370700", "370702", "370703", "370704", "370705", "370724", "370725", "370781", "370782", "370783", "370784", "370785", "370786"},
    {"370800", "370811", "370812", "370826", "370827", "370828", "370829", "370830", "370831", "370832", "370881", "370883"},
    {"370900", "370902", "370911", "370921", "370923", "370982", "370983"},
    {"371000", "371002", "371081", "371082", "371083", "371998", "371999"},
    {"371100", "371102", "371103", "371121", "371122"},
    {"371300", "371302", "371311", "371312", "371321", "371322", "371323", "371324", "371325", "371326", "371327", "371328", "371329"},
    {"371400", "371402", "371403", "371422", "371423", "371424", "371425", "371426", "371427", "371428", "371481", "371482"},
    {"371500", "371502", "371521", "371522", "371523", "371524", "371525", "371526", "371581"},
    {"371600", "371602", "371603", "371621", "371622", "371623", "371625", "371681"},
    {"371700", "371702", "371703", "371721", "371722", "371723", "371724", "371725", "371726", "371728"},
    // 河南
    {"410100", "410102", "410103", "410104", "410105", "410106", "410108", "410122", "410181", "410182", "410183", "410184", "410185"},
    {"410200", "410202", "410203", "410204", "410205", "410212", "410221", "410222", "410223", "410225"},
    {"410300", "410302", "410303", "410304", "410305", "410306", "410311", "410322", "410323", "410324", "410325", "410326", "410327", "410328", "410329", "410381"},
    {"410400", "410402", "410403", "410404", "410411", "410421", "410422", "410423", "410425", "410481", "410482"},
    {"410500", "410502", "410503", "410505", "410506", "410523", "410526", "410527", "410581"},
    {"410600", "410602", "410603", "410611", "410621", "410622"},
    {"410700", "410702", "410703", "410704", "410711", "410724", "410725", "410726", "410727", "410728", "410781", "410782"},
    {"410800", "410802", "410803", "410804", "410811", "410821", "410822", "410823", "410825", "410882", "410883"},
    {"410900", "410902", "410922", "410923", "410926", "410927"},
    {"411000", "411002", "411003", "411024", "411025", "411081", "411082"},
    {"411100", "411102", "411103", "411104", "411121", "411122"},
    {"411200", "411202", "411203", "411221", "411224", "411281", "411282"},
    {"411300", "411302", "411303", "411321", "411322", "411323", "411324", "411325", "411326", "411327", "411328", "411329", "411330", "411381"},
    {"411400", "411402", "411403", "411421", "411422", "411423", "411424", "411425", "411426", "411481"},
    {"411500", "411502", "411503", "411521", "411522", "411523", "411524", "411525", "411526", "411527", "411528"},
    {"411600", "411602", "411621", "411622", "411623", "411624", "411625", "411626", "411627", "411628", "411681"},
    {"411700", "411702", "411721", "411722", "411723", "411724", "411725", "411726", "411727", "411728", "411729"},
    {"419001"},
    // 湖北
    {"420100", "420102", "420103", "420104", "420105", "420106", "420107", "420111", "420112", "420113", "420114", "420115", "420116", "420117"},
    {"420200", "420202", "420203", "420204", "420205", "420222", "420281"},
    {"420300", "420302", "420303", "420304", "420322", "420323", "420324", "420325", "420381"},
    {"420500", "420502", "420503", "420504", "420505", "420506", "4205064010", "420525", "420526", "420527", "420528", "420529", "420581", "420582", "420583"},
    {"420600", "420602", "420606", "420607", "420624", "420625", "420626", "420682", "420683", "420684"},
    {"420700", "420702", "420703", "420704"},
    {"420800", "420802", "420804", "420822", "420881", "420882"},
    {"420900", "420902", "420921", "420922", "420923", "420981", "420982", "420984"},
    {"421000", "421002", "421022", "421023", "421024", "421081", "421083", "421087"},
    {"421100", "421102", "421121", "421122", "421123", "421124", "421125", "421126", "421127", "421181", "421182"},
    {"421200", "421202", "421221", "421222", "421223", "421224", "421281"},
    {"421300", "421303", "421321", "421381"},
    {"422800", "422802", "422822", "422823", "422825", "422826", "422827", "422828"},
    {"429004"},
    {"429005"},
    {"429006"},
    {"429021"},
    // 湖南
    {"430100", "430102", "430103", "430104", "430105", "430111", "430112", "430121", "430181", "430182", "430999"},
    {"430200", "430202", "430203", "430204", "430211", "430223", "430224", "430225", "430281"},
    {"430300", "430302", "430304", "430381", "430382"},
    {"430400", "430405", "430406", "430407", "430408", "430412", "430421", "430422", "430423", "430424", "430426", "430481", "430482"},
    {"430500", "430502", "430503", "430511", "430521", "430522", "430523", "430524", "430525", "430527", "430528", "430529", "430581"},
    {"430600", "430602", "430603", "430611", "430623", "430624", "430626", "430681", "430682"},
    {"430700", "430702", "430703", "430721", "430722", "430723", "430724", "430725", "430726", "430781"},
    {"430800", "430802", "430811", "430821", "430822"},
    {"430900", "430902", "430903", "430921", "430922", "430923", "430981"},
    {"431000", "431002", "431003", "431021", "431022", "431023", "431024", "431025", "431026", "431027", "431028", "431081"},
    {"431100", "431102", "431103", "431121", "431122", "431123", "431124", "431125", "431126", "431127", "431128", "431129"},
    {"431200", "431202", "431221", "431222", "431223", "431224", "431225", "431226", "431227", "431228", "431229", "431230", "431281"},
    {"431300", "431302", "431321", "431322", "431381", "431382"},
    {"433100", "433101", "433122", "433123", "433124", "433125", "433126", "433127", "433130"},
    // 广东
    {"440100", "440103", "440104", "440105", "440106", "440111", "440112", "440113", "440114", "440115", "440117", "440118"},
    {"440200", "440203", "440204", "440205", "440222", "440224", "440229", "440232", "440233", "440281", "440282"},
    {"440300", "440303", "440304", "440305", "440306", "440307", "440308", "440309", "440310", "440311"},
    {"440400", "440402", "440403", "440404"},
    {"440500", "440507", "440511", "440512", "440513", "440514", "440515", "440523"},
    {"440600", "440604", "440605", "440606", "440607", "440608"},
    {"440700", "440703", "440704", "440705", "440781", "440783", "440784", "440785"},
    {"440800", "440802", "440803", "440804", "440811", "440823", "440825", "440881", "440882", "440883"},
    {"440900", "440902", "440904", "440981", "440982", "440983"},
    {"441200", "441202", "441203", "441204", "441223", "441224", "441225", "441226", "441284"},
    {"441300", "441302", "441303", "441322", "441323", "441324"},
    {"441400", "441402", "441403", "441422", "441423", "441424", "441426", "441427", "441481"},
    {"441500", "441521", "441523", "441581"},
    {"441600", "441602", "441621", "441622", "441623", "441624", "441625"},
    {"441700", "441702", "441704", "441721", "441781"},
    {"441800", "441802", "441803", "441821", "441823", "441825", "441826", "441881", "441882"},
    {"441900"},
    {"442000"},
    {"445100", "445102", "445103", "445122"},
    {"445200", "445202", "445203", "445222", "445224", "445281"},
    {"445300", "445302", "445303", "445321", "445322", "445381"},
    // 广西
    {"450100", "450102", "450103", "450105", "450107", "450108", "450109", "450110", "450123", "450124", "450125", "450126", "450127"},
    {"450200", "450202", "450203", "450204", "450205", "450206", "450222", "450223", "450224", "450225", "450226"},
    {"450300", "450302", "450303", "450304", "450305", "450311", "450312", "450321", "450323", "450324", "450325", "450326", "450327", "450328", "450329", "450330", "450332", "450381"},
    {"450400", "450403", "450405", "450406", "450421", "450422", "450423", "450481"},
    {"450500", "450502", "45050210", "450503", "450512", "450521"},
    {"450600", "450602", "450603", "450621", "450681"},
    {"450700", "450702", "450703", "450721", "450722"},
    {"450800", "450802", "450803", "450804", "450821", "450881"},
    {"450900", "450902", "450903", "450921", "450922", "450923", "450924", "450981"},
    {"451000", "451002", "451003", "451022", "451023", "451024", "451026", "451027", "451028", "451029", "451030", "451031", "451081"},
    {"451100", "451102", "451103", "451121", "451122", "451123"},
    {"451200", "451202", "451203", "451221", "451222", "451223", "451224", "451225", "451226", "451227", "451228", "451229"},
    {"451300", "451302", "451321", "451322", "451323", "451324", "451381"},
    {"451400", "451402", "451421", "451422", "451423", "451424", "451425", "451481"},
    // 海南
    {"460100", "460105", "460106", "460107", "460108"},
    {"460200", "460202", "460203", "460204", "460205"},
    {"460300", "460321", "460322", "460323"},
    {"460400"},
    {"469001"},
    {"469002"},
    {"469005"},
    {"469006"},
    {"469007"},
    {"469021"},
    {"469022"},
    {"469023"},
    {"469024"},
    {"469025"},
    {"469026"},
    {"469027"},
    {"469028"},
    {"469029"},
    {"469030"},
    // 重庆
    {"500000", "500101", "500102", "500109", "500110", "500111", "500112", "500113", "500114", "500115", "500116", "500117", "500118", "500119", "500120", "500151", "500152", "500153", "500154", "500155", "500156", "500229", "500230", "500231", "500233", "500235", "500236", "500237", "500238", "500240", "500241", "500242", "500243"},
    {"500103"},
    {"500104"},
    {"500105"},
    {"500106"},
    {"500107"},
    {"500108"},
    {"500154"},
    // 四川
    {"510100", "510104", "510105", "510106", "510107", "510108", "510112", "510113", "510114", "510115", "510116", "510117", "510121", "510129", "510131", "510132", "510181", "510182", "510183", "510184", "510185"},
    {"510300", "510302", "510303", "510304", "510311", "510321", "510322"},
    {"510400", "510402", "510403", "510411", "510421", "510422"},
    {"510500", "510502", "510503", "510504", "510521", "510522", "510524", "510525"},
    {"510600", "510603", "510604", "510623", "510681", "510682", "510683"},
    {"510700", "510703", "510704", "510705", "510722", "510723", "510725", "510726", "510727", "510781"},
    {"510800", "510802", "510811", "510812", "510821", "510822", "510823", "510824"},
    {"510900", "510903", "510904", "510921", "510922", "510923"},
    {"511000", "511002", "511011", "511024", "511025", "511083"},
    {"511100", "511102", "511111", "511112", "511113", "511123", "511124", "511126", "511129", "511132", "511133", "511181"},
    {"511300", "511302", "511303", "511304", "511321", "511322", "511323", "511324", "511325", "511381"},
    {"511400", "511402", "511403", "511421", "511423", "511424", "511425"},
    {"511500", "511502", "511503", "511504", "511523", "511524", "511525", "511526", "511527", "511528", "511529"},
    {"511600", "511603", "511621", "511622", "511623", "511681"},
    {"511700", "511702", "511703", "511722", "511723", "511724", "511725", "511781"},
    {"511800", "511802", "511803", "511822", "511823", "511824", "511825", "511826", "511827"},
    {"511900", "511902", "511903", "511921", "511922", "511923"},
    {"512000", "512002", "512021", "512022"},
    {"513200", "513201", "513221", "513222", "513223", "513224", "513225", "513226", "513227", "513228", "513230", "513232", "513233"},
    {"513300", "513301", "513322", "513323", "513324", "513325", "513326", "513327", "513329", "513330", "513331", "513332", "513333", "513334", "513335", "513336", "513337", "513338"},
    {"513400", "513401", "513422", "513423", "513424", "513425", "513426", "513427", "513428", "513429", "513430", "513431", "513432", "513433", "513434", "513435", "513436", "513437"},
    // 贵州
    {"520100", "520102", "520103", "520111", "520112", "520113", "520115", "520121", "520122", "520123", "520181"},
    {"520200", "520201", "520203", "520221", "520281"},
    {"520300", "520302", "520303", "520304", "520322", "520323", "520324", "520325", "520326", "520327", "520328", "520329", "520330", "520381", "520382"},
    {"520400", "520402", "520403", "520422", "520423", "520424", "520425"},
    {"520500", "520502", "520521", "520522", "520523", "520524", "520525", "520526", "520527"},
    {"520600", "520602", "520603", "520621", "520622", "520623", "520624", "520625", "520626", "520627", "520628"},
    {"522300", "522301", "522302", "522323", "522324", "522325", "522326", "522327", "522328"},
    {"522600", "522601", "522622", "522623", "522624", "522625", "522626", "522627", "522628", "522629", "522630", "522631", "522632", "522633", "522634", "522635", "522636"},
    {"522700", "522701", "522702", "522722", "522723", "522725", "522726", "522727", "522728", "522729", "522730", "522731", "522732"},
    // 云南
    {"530100", "530102", "530103", "530111", "530112", "530113", "530114", "530115", "530124", "530125", "530126", "530127", "530128", "530129", "530181"},
    {"530300", "530302", "530303", "530304", "530322", "530323", "530324", "530325", "530326", "530381"},
    {"530400", "530402", "530403", "530422", "530423", "530424", "530425", "530426", "530427", "530428"},
    {"530500", "530502", "530521", "530523", "530524", "530581"},
    {"530600", "530602", "530621", "530622", "530623", "530624", "530625", "530626", "530627", "530628", "530629", "530681"},
    {"530700", "530702", "530721", "530722", "530723", "530724"},
    {"530800", "530802", "530821", "530822", "530823", "530824", "530825", "530826", "530827", "530828", "530829"},
    {"530900", "530902", "530921", "530922", "530923", "530924", "530925", "530926", "530927"},
    {"532300", "532322", "532323", "532324", "532325", "532326", "532327", "532328", "532329", "532331"},
    {"532500", "532501", "532502", "532503", "532504", "532523", "532524", "532525", "532527", "532528", "532530", "532531", "532532"},
    {"532600", "532622", "532623", "532624", "532625", "532626", "532627", "532628"},
    {"532800", "532801", "532822", "532823"},
    {"532901", "532922", "532923", "532924", "532925", "532926", "532927", "532928", "532929", "532930", "532931", "532932"},
    {"533100", "533102", "533103", "533122", "533123", "533124"},
    {"533300", "533301", "533323", "533324", "533325"},
    {"533400", "533401", "533422", "533423"},
    // 西藏
    {"540100", "540102", "540103", "540104", "540121", "540122", "540123", "540124", "540127"},
    {"540200", "540202", "540221", "540222", "540223", "540224", "540225", "540226", "540227", "540228", "540229", "540230", "540231", "540232", "540233", "5402331010", "540234", "540235", "540236", "540237"},
    {"540300", "540302", "540321", "540322", "540323", "540324", "540325", "540326", "540327", "540328", "540329", "540330"},
    {"540400", "540402", "540421", "540422", "540423", "540424", "540425", "540426"},
    {"540500", "540502", "54050210", "540521", "540522", "540523", "540524", "540525", "540526", "540527", "540528", "540529", "540530", "540531"},
    {"540600", "540602", "540621", "540622", "540623", "540624", "540625", "540626", "540627", "540628", "540629", "540630"},
    {"542500", "542521", "542522", "542523", "54252310", "542524", "542525", "542526", "542527"},
    // 陕西
    {"610100", "610102", "610103", "610104", "610111", "610112", "610113", "610114", "610115", "610116", "610117", "610118", "610122", "610124"},
    {"610200", "610202", "610203", "610204", "610222"},
    {"610300", "610302", "610303", "610304", "610322", "610323", "610324", "610326", "610327", "610328", "610329", "610330", "610331"},
    {"611100"},
    {"610400", "610402", "610403", "610404", "610422", "610423", "610424", "610425", "610426", "610428", "610429", "610430", "610431", "610481", "610482"},
    {"610500", "610502", "610503", "610522", "610523", "610524", "610525", "610526", "610527", "610528", "610581", "610582"},
    {"610600", "610602", "610603", "610621", "610622", "610623", "610625", "610626", "610627", "610628", "610629", "610630", "610631", "610632"},
    {"610700", "610702", "610703", "610722", "610723", "610724", "610725", "610726", "610727", "610728", "610729", "610730"},
    {"610800", "610802", "610803", "610822", "610824", "610825", "610826", "610827", "610828", "610829", "610830", "610831", "610881"},
    {"610900", "610902", "610921", "610922", "610923", "610924", "610925", "610926", "610927", "610928", "610929"},
    {"611000", "611002", "611021", "611022", "611023", "611024", "611025", "611026"},
    // 甘肃
    {"620100", "620102", "620103", "620104", "620105", "620111", "620121", "620122", "620123"},
    {"620200"},
    {"620300", "620302", "620321"},
    {"620400", "620403", "620421", "620422", "620423"},
    {"620500", "620502", "620503", "620521", "620522", "620523", "620524", "620525"},
    {"620600", "620602", "620621", "620622", "620623"},
    {"620700", "620702", "620721", "620722", "620723", "620724", "620725"},
    {"620800", "620802", "620821", "620822", "620823", "620825", "620826", "620881"},
    {"620900", "620902", "620921", "620922", "620923", "620924", "620981", "620982"},
    {"621000", "621002", "621021", "621022", "621023", "621024", "621025", "621026", "621027"},
    {"621100", "621102", "621121", "621122", "621123", "621124", "621125", "621126"},
    {"621200", "621202", "621221", "621222", "621223", "621224", "621225", "621226", "621227", "621228"},
    {"622900", "622922", "622923", "622924", "622925", "622926", "622927"},
    {"623000", "623001", "623021", "623022", "623023", "623024", "623025", "623026", "623027"},
    // 青海
    {"630100", "630102", "630103", "630104", "630105", "630121", "630122", "630123"},
    {"630200", "630202", "630203", "630222", "630223", "630224", "630225"},
    {"632200", "632221", "632222", "632223", "632224"},
    {"632300", "632321", "632322", "632323", "632324"},
    {"632500", "632521", "632522", "632523", "632524", "632525"},
    {"632600", "632621", "632622", "632623", "632624", "632625", "632626"},
    {"632700", "632722", "632723", "632724", "632725", "632726"},
    {"632800", "632801", "632802", "632803", "6328031020", "632821", "632822", "632823", "632857"},
    // 宁夏
    {"640100", "640104", "640105", "640106", "640121", "640122", "640181"},
    {"640200", "640202", "640205", "640221", "6402211060"},
    {"640300", "640302", "640303", "640323", "640324", "640381"},
    {"640400", "640402", "640422", "640423", "640424", "640425"},
    {"640500", "640502", "640521", "640522"},
    // 新疆
    {"650100", "650102", "650103", "650104", "650105", "650106", "650107", "650109", "650121", "6501212082", "650998", "650999"},
    {"650200", "650202", "650204", "650205"},
    {"650400", "650402", "650421", "650422"},
    {"650500", "650502", "650521", "650522"},
    {"652300", "652302", "652323", "652324", "652325", "652327", "652328", "652399"},
    {"652700", "652701", "652702", "652722", "652723"},
    {"652800", "652801", "652822", "652823", "652824", "6528241040", "652825", "652825103", "652826", "652827", "6528271010", "6528271040", "652828", "652829"},
    {"652900", "652922", "652923", "652924", "652925", "652926", "652927", "652928", "652929", "652999"},
    {"653000", "653001", "653022", "653023", "653024"},
    {"653100", "653121", "653122", "653123", "653124", "653125", "653126", "653127", "653128", "653129", "653130", "653131"},
    {"653200", "653222", "653223", "653224", "653225", "653226", "653227"},
    {"654000", "654002", "654003", "654004", "654021", "654022", "654023", "654024", "654025", "654026", "654027", "654028"},
    {"654200", "654202", "654221", "654223", "654224", "654225", "654226"},
    {"654300", "654321", "654322", "654323", "654324", "654325", "654326"},
    {"659001", "6590011010", "6590019999"},
    {"659002"},
    {"659003"},
    {"659004"},
    {"659005"},
    {"659006"},
    {"659007"},
    {"659008"},
    {"659009"},
    // 台湾
    {"710100", "712200", "712300", "712400"},
    {"710200", "710500", "710700", "713300", "713400"},
    {"710400", "712500", "712700", "712800", "712900", "713500"},
    // 香港
    {"810100"},
    {"810200"},
    {"810300"},
    // 澳门
    {"820100"},
    {"820200"},
    {"820300"},
};

// 处理返回按钮点击事件
static void return_handler(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t *target = lv_event_get_target(e);
    lv_ui *ui = (lv_ui *)lv_event_get_user_data(e);

    if (code == LV_EVENT_CLICKED)
    {
        setup_scr_screen_set_hor(&guider_ui);
        lv_scr_load_anim(guider_ui.screen_set_hor, LV_SCR_LOAD_ANIM_NONE, 0, 0, true);
    }
}

static void select_city_handler(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t *target = lv_event_get_target(e);

    if (code == LV_EVENT_VALUE_CHANGED)
    {
        uint16_t city_3_value = 0;
        uint16_t id = lv_dropdown_get_selected(guider_ui.screen_city_hor_ddlist_1);
        for (int i = 0; i < id; i++)
            city_3_value += city_cnt[i];

        if (target == guider_ui.screen_city_hor_ddlist_1)
        {
            if (MY_SET.language == LANGUAGE_CN)
            {
                lv_dropdown_set_options(guider_ui.screen_city_hor_ddlist_2, my_city_2[id].city_name);
                lv_dropdown_set_options(guider_ui.screen_city_hor_ddlist_3, my_city_3[city_3_value].city_name);
            }else{
                lv_dropdown_set_options(guider_ui.screen_city_hor_ddlist_2, my_city_2_en[id].city_name);
                lv_dropdown_set_options(guider_ui.screen_city_hor_ddlist_3, my_city_3_en[city_3_value].city_name);
            }
        }
        else if (target == guider_ui.screen_city_hor_ddlist_2)
        {
            uint16_t city_2_value = lv_dropdown_get_selected(guider_ui.screen_city_hor_ddlist_2);
            if (MY_SET.language == LANGUAGE_CN)
                lv_dropdown_set_options(guider_ui.screen_city_hor_ddlist_3, my_city_3[city_3_value + city_2_value].city_name);
            else
                lv_dropdown_set_options(guider_ui.screen_city_hor_ddlist_3, my_city_3_en[city_3_value + city_2_value].city_name);
        }
    }
}

static void ok_handler(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t *target = lv_event_get_target(e);

    uint8_t city_id[3] = {0};
    char buf[32];
    if (code == LV_EVENT_CLICKED)
    {
        my_city_id.city_1 = lv_dropdown_get_selected(guider_ui.screen_city_hor_ddlist_1);
        my_city_id.city_2 = lv_dropdown_get_selected(guider_ui.screen_city_hor_ddlist_2);
        my_city_id.city_3 = lv_dropdown_get_selected(guider_ui.screen_city_hor_ddlist_3);
        if(city_id[0] != my_city_id.city_1 || city_id[1] != my_city_id.city_2 || city_id[2] != my_city_id.city_3)
        {
            start_set_lwip_flag = true;
            city_id[0] = my_city_id.city_1;
            city_id[1] = my_city_id.city_2;
            city_id[2] = my_city_id.city_3;
        }
        rt_kprintf("my_city_id.city_3 = %d\n", my_city_id.city_3);
        uint16_t city_value = 0;
        for (int i = 0; i < my_city_id.city_1; i++)
            city_value += city_cnt[i];
        my_city_id.city_value = city_value;
        lv_dropdown_get_selected_str(guider_ui.screen_city_hor_ddlist_1, buf, sizeof(buf));
        rt_kprintf("选择的城市是：%s", buf);
        lv_dropdown_get_selected_str(guider_ui.screen_city_hor_ddlist_2, buf, sizeof(buf));
        rt_kprintf("-%s", buf);
        lv_dropdown_get_selected_str(guider_ui.screen_city_hor_ddlist_3, buf, sizeof(buf));
        rt_kprintf("-%s\n", buf);
        rt_kprintf("city_code : %s\n", city_code[my_city_id.city_value +  my_city_id.city_2][my_city_id.city_3]);
        memset(city_name_str, 0, sizeof(city_name_str));
        sprintf(city_name_str,"%s",buf);


        city3_temp = 0;
        uint16_t id = lv_dropdown_get_selected(guider_ui.screen_city_hor_ddlist_1);
        city2_temp = lv_dropdown_get_selected(guider_ui.screen_city_hor_ddlist_2);
        for (int i = 0; i < id; i++)
            city3_temp += city_cnt[i];

        setup_scr_screen_set_hor(&guider_ui);
        lv_scr_load_anim(guider_ui.screen_set_hor, LV_SCR_LOAD_ANIM_NONE, 0, 0, true);
        save_begin();
    }
}

void setup_scr_screen_city_hor(lv_ui *ui)
{
    // Write codes screen_city_hor
    ui->screen_city_hor = lv_obj_create(NULL);
    lv_obj_set_size(ui->screen_city_hor, 1024, 768);
    lv_obj_set_scrollbar_mode(ui->screen_city_hor, LV_SCROLLBAR_MODE_OFF);

    // Write style for screen_city_hor, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_city_hor, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_city_hor, lv_color_hex(0xc3c3c3), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_city_hor, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write codes screen_city_hor_btn_return
    ui->screen_city_hor_btn_return = lv_btn_create(ui->screen_city_hor);
    ui->screen_city_hor_btn_return_label = lv_label_create(ui->screen_city_hor_btn_return);
    lv_label_set_text(ui->screen_city_hor_btn_return_label, "返回");
    lv_label_set_long_mode(ui->screen_city_hor_btn_return_label, LV_LABEL_LONG_WRAP);
    lv_obj_align(ui->screen_city_hor_btn_return_label, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_pad_all(ui->screen_city_hor_btn_return, 0, LV_STATE_DEFAULT);
    lv_obj_set_width(ui->screen_city_hor_btn_return_label, LV_PCT(100));
    lv_obj_set_pos(ui->screen_city_hor_btn_return, 436, 704);
    lv_obj_set_size(ui->screen_city_hor_btn_return, 154, 53);

    // Write style for screen_city_hor_btn_return, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_text_color(ui->screen_city_hor_btn_return, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_city_hor_btn_return, &lv_font_Dengb_24, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_city_hor_btn_return, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_city_hor_btn_return, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->screen_city_hor_btn_return, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->screen_city_hor_btn_return, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->screen_city_hor_btn_return, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->screen_city_hor_btn_return, LV_BORDER_SIDE_BOTTOM | LV_BORDER_SIDE_RIGHT, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_city_hor_btn_return, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_city_hor_btn_return, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_city_hor_btn_return, lv_color_hex(0xff0027), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_city_hor_btn_return, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_city_hor_btn_return, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write codes screen_city_hor_ddlist_1
    ui->screen_city_hor_ddlist_1 = lv_dropdown_create(ui->screen_city_hor);
    lv_dropdown_set_options(ui->screen_city_hor_ddlist_1, "");
    lv_obj_set_pos(ui->screen_city_hor_ddlist_1, 42, 110);
    lv_obj_set_size(ui->screen_city_hor_ddlist_1, 254, 57);

    // Write style for screen_city_hor_ddlist_1, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_text_color(ui->screen_city_hor_ddlist_1, lv_color_hex(0x0D3055), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_city_hor_ddlist_1, &lv_font_Dengb_24, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_city_hor_ddlist_1, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->screen_city_hor_ddlist_1, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->screen_city_hor_ddlist_1, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->screen_city_hor_ddlist_1, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->screen_city_hor_ddlist_1, LV_BORDER_SIDE_FULL, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_city_hor_ddlist_1, 13, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_city_hor_ddlist_1, 6, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_city_hor_ddlist_1, 6, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_city_hor_ddlist_1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_city_hor_ddlist_1, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_city_hor_ddlist_1, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_city_hor_ddlist_1, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_city_hor_ddlist_1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write style state: LV_STATE_CHECKED for &style_screen_city_hor_ddlist_1_extra_list_selected_checked
    static lv_style_t style_screen_city_hor_ddlist_1_extra_list_selected_checked;
    ui_init_style(&style_screen_city_hor_ddlist_1_extra_list_selected_checked);

    lv_style_set_border_width(&style_screen_city_hor_ddlist_1_extra_list_selected_checked, 2);
    lv_style_set_border_opa(&style_screen_city_hor_ddlist_1_extra_list_selected_checked, 255);
    lv_style_set_border_color(&style_screen_city_hor_ddlist_1_extra_list_selected_checked, lv_color_hex(0x2f3bff));
    lv_style_set_border_side(&style_screen_city_hor_ddlist_1_extra_list_selected_checked, LV_BORDER_SIDE_FULL);
    lv_style_set_radius(&style_screen_city_hor_ddlist_1_extra_list_selected_checked, 2);
    lv_style_set_bg_opa(&style_screen_city_hor_ddlist_1_extra_list_selected_checked, 255);
    lv_style_set_bg_color(&style_screen_city_hor_ddlist_1_extra_list_selected_checked, lv_color_hex(0x00a1b5));
    lv_style_set_bg_grad_dir(&style_screen_city_hor_ddlist_1_extra_list_selected_checked, LV_GRAD_DIR_NONE);
    lv_obj_add_style(lv_dropdown_get_list(ui->screen_city_hor_ddlist_1), &style_screen_city_hor_ddlist_1_extra_list_selected_checked, LV_PART_SELECTED | LV_STATE_CHECKED);

    // Write style state: LV_STATE_DEFAULT for &style_screen_city_hor_ddlist_1_extra_list_main_default
    static lv_style_t style_screen_city_hor_ddlist_1_extra_list_main_default;
    ui_init_style(&style_screen_city_hor_ddlist_1_extra_list_main_default);

    lv_style_set_max_height(&style_screen_city_hor_ddlist_1_extra_list_main_default, 235);
    lv_style_set_text_color(&style_screen_city_hor_ddlist_1_extra_list_main_default, lv_color_hex(0x0D3055));
    lv_style_set_text_font(&style_screen_city_hor_ddlist_1_extra_list_main_default, &lv_font_Dengb_24);
    lv_style_set_text_opa(&style_screen_city_hor_ddlist_1_extra_list_main_default, 255);
    lv_style_set_border_width(&style_screen_city_hor_ddlist_1_extra_list_main_default, 0);
    lv_style_set_radius(&style_screen_city_hor_ddlist_1_extra_list_main_default, 3);
    lv_style_set_bg_opa(&style_screen_city_hor_ddlist_1_extra_list_main_default, 255);
    lv_style_set_bg_color(&style_screen_city_hor_ddlist_1_extra_list_main_default, lv_color_hex(0xe7daee));
    lv_style_set_bg_grad_dir(&style_screen_city_hor_ddlist_1_extra_list_main_default, LV_GRAD_DIR_NONE);
    lv_obj_add_style(lv_dropdown_get_list(ui->screen_city_hor_ddlist_1), &style_screen_city_hor_ddlist_1_extra_list_main_default, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write style state: LV_STATE_DEFAULT for &style_screen_city_hor_ddlist_1_extra_list_scrollbar_default
    static lv_style_t style_screen_city_hor_ddlist_1_extra_list_scrollbar_default;
    ui_init_style(&style_screen_city_hor_ddlist_1_extra_list_scrollbar_default);

    lv_style_set_radius(&style_screen_city_hor_ddlist_1_extra_list_scrollbar_default, 0);
    lv_style_set_bg_opa(&style_screen_city_hor_ddlist_1_extra_list_scrollbar_default, 255);
    lv_style_set_bg_color(&style_screen_city_hor_ddlist_1_extra_list_scrollbar_default, lv_color_hex(0x007fff));
    lv_style_set_bg_grad_dir(&style_screen_city_hor_ddlist_1_extra_list_scrollbar_default, LV_GRAD_DIR_NONE);
    lv_obj_add_style(lv_dropdown_get_list(ui->screen_city_hor_ddlist_1), &style_screen_city_hor_ddlist_1_extra_list_scrollbar_default, LV_PART_SCROLLBAR | LV_STATE_DEFAULT);

    // Write codes screen_city_hor_ddlist_2
    ui->screen_city_hor_ddlist_2 = lv_dropdown_create(ui->screen_city_hor);
    lv_dropdown_set_options(ui->screen_city_hor_ddlist_2, "");
    lv_obj_set_pos(ui->screen_city_hor_ddlist_2, 383, 110);
    lv_obj_set_size(ui->screen_city_hor_ddlist_2, 254, 57);

    // Write style for screen_city_hor_ddlist_2, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_text_color(ui->screen_city_hor_ddlist_2, lv_color_hex(0x0D3055), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_city_hor_ddlist_2, &lv_font_Dengb_24, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_city_hor_ddlist_2, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->screen_city_hor_ddlist_2, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->screen_city_hor_ddlist_2, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->screen_city_hor_ddlist_2, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->screen_city_hor_ddlist_2, LV_BORDER_SIDE_FULL, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_city_hor_ddlist_2, 13, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_city_hor_ddlist_2, 6, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_city_hor_ddlist_2, 6, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_city_hor_ddlist_2, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_city_hor_ddlist_2, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_city_hor_ddlist_2, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_city_hor_ddlist_2, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_city_hor_ddlist_2, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write style state: LV_STATE_CHECKED for &style_screen_city_hor_ddlist_2_extra_list_selected_checked
    static lv_style_t style_screen_city_hor_ddlist_2_extra_list_selected_checked;
    ui_init_style(&style_screen_city_hor_ddlist_2_extra_list_selected_checked);

    lv_style_set_border_width(&style_screen_city_hor_ddlist_2_extra_list_selected_checked, 2);
    lv_style_set_border_opa(&style_screen_city_hor_ddlist_2_extra_list_selected_checked, 255);
    lv_style_set_border_color(&style_screen_city_hor_ddlist_2_extra_list_selected_checked, lv_color_hex(0x2f3bff));
    lv_style_set_border_side(&style_screen_city_hor_ddlist_2_extra_list_selected_checked, LV_BORDER_SIDE_FULL);
    lv_style_set_radius(&style_screen_city_hor_ddlist_2_extra_list_selected_checked, 2);
    lv_style_set_bg_opa(&style_screen_city_hor_ddlist_2_extra_list_selected_checked, 255);
    lv_style_set_bg_color(&style_screen_city_hor_ddlist_2_extra_list_selected_checked, lv_color_hex(0x00a1b5));
    lv_style_set_bg_grad_dir(&style_screen_city_hor_ddlist_2_extra_list_selected_checked, LV_GRAD_DIR_NONE);
    lv_obj_add_style(lv_dropdown_get_list(ui->screen_city_hor_ddlist_2), &style_screen_city_hor_ddlist_2_extra_list_selected_checked, LV_PART_SELECTED | LV_STATE_CHECKED);

    // Write style state: LV_STATE_DEFAULT for &style_screen_city_hor_ddlist_2_extra_list_main_default
    static lv_style_t style_screen_city_hor_ddlist_2_extra_list_main_default;
    ui_init_style(&style_screen_city_hor_ddlist_2_extra_list_main_default);

    lv_style_set_max_height(&style_screen_city_hor_ddlist_2_extra_list_main_default, 235);
    lv_style_set_text_color(&style_screen_city_hor_ddlist_2_extra_list_main_default, lv_color_hex(0x0D3055));
    lv_style_set_text_font(&style_screen_city_hor_ddlist_2_extra_list_main_default, &lv_font_Dengb_24);
    lv_style_set_text_opa(&style_screen_city_hor_ddlist_2_extra_list_main_default, 255);
    lv_style_set_border_width(&style_screen_city_hor_ddlist_2_extra_list_main_default, 0);
    lv_style_set_radius(&style_screen_city_hor_ddlist_2_extra_list_main_default, 3);
    lv_style_set_bg_opa(&style_screen_city_hor_ddlist_2_extra_list_main_default, 255);
    lv_style_set_bg_color(&style_screen_city_hor_ddlist_2_extra_list_main_default, lv_color_hex(0xe7daee));
    lv_style_set_bg_grad_dir(&style_screen_city_hor_ddlist_2_extra_list_main_default, LV_GRAD_DIR_NONE);
    lv_obj_add_style(lv_dropdown_get_list(ui->screen_city_hor_ddlist_2), &style_screen_city_hor_ddlist_2_extra_list_main_default, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write style state: LV_STATE_DEFAULT for &style_screen_city_hor_ddlist_2_extra_list_scrollbar_default
    static lv_style_t style_screen_city_hor_ddlist_2_extra_list_scrollbar_default;
    ui_init_style(&style_screen_city_hor_ddlist_2_extra_list_scrollbar_default);

    lv_style_set_radius(&style_screen_city_hor_ddlist_2_extra_list_scrollbar_default, 0);
    lv_style_set_bg_opa(&style_screen_city_hor_ddlist_2_extra_list_scrollbar_default, 255);
    lv_style_set_bg_color(&style_screen_city_hor_ddlist_2_extra_list_scrollbar_default, lv_color_hex(0x007fff));
    lv_style_set_bg_grad_dir(&style_screen_city_hor_ddlist_2_extra_list_scrollbar_default, LV_GRAD_DIR_NONE);
    lv_obj_add_style(lv_dropdown_get_list(ui->screen_city_hor_ddlist_2), &style_screen_city_hor_ddlist_2_extra_list_scrollbar_default, LV_PART_SCROLLBAR | LV_STATE_DEFAULT);

    // Write codes screen_city_hor_ddlist_3
    ui->screen_city_hor_ddlist_3 = lv_dropdown_create(ui->screen_city_hor);
    lv_dropdown_set_options(ui->screen_city_hor_ddlist_3, "");
    lv_obj_set_pos(ui->screen_city_hor_ddlist_3, 717, 110);
    lv_obj_set_size(ui->screen_city_hor_ddlist_3, 254, 57);

    // Write style for screen_city_hor_ddlist_3, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_text_color(ui->screen_city_hor_ddlist_3, lv_color_hex(0x0D3055), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_city_hor_ddlist_3, &lv_font_Dengb_24, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_city_hor_ddlist_3, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->screen_city_hor_ddlist_3, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->screen_city_hor_ddlist_3, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->screen_city_hor_ddlist_3, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->screen_city_hor_ddlist_3, LV_BORDER_SIDE_FULL, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_city_hor_ddlist_3, 13, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_city_hor_ddlist_3, 6, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_city_hor_ddlist_3, 6, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_city_hor_ddlist_3, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_city_hor_ddlist_3, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_city_hor_ddlist_3, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_city_hor_ddlist_3, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_city_hor_ddlist_3, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write style state: LV_STATE_CHECKED for &style_screen_city_hor_ddlist_3_extra_list_selected_checked
    static lv_style_t style_screen_city_hor_ddlist_3_extra_list_selected_checked;
    ui_init_style(&style_screen_city_hor_ddlist_3_extra_list_selected_checked);

    lv_style_set_border_width(&style_screen_city_hor_ddlist_3_extra_list_selected_checked, 2);
    lv_style_set_border_opa(&style_screen_city_hor_ddlist_3_extra_list_selected_checked, 255);
    lv_style_set_border_color(&style_screen_city_hor_ddlist_3_extra_list_selected_checked, lv_color_hex(0x2f3bff));
    lv_style_set_border_side(&style_screen_city_hor_ddlist_3_extra_list_selected_checked, LV_BORDER_SIDE_FULL);
    lv_style_set_radius(&style_screen_city_hor_ddlist_3_extra_list_selected_checked, 2);
    lv_style_set_bg_opa(&style_screen_city_hor_ddlist_3_extra_list_selected_checked, 255);
    lv_style_set_bg_color(&style_screen_city_hor_ddlist_3_extra_list_selected_checked, lv_color_hex(0x00a1b5));
    lv_style_set_bg_grad_dir(&style_screen_city_hor_ddlist_3_extra_list_selected_checked, LV_GRAD_DIR_NONE);
    lv_obj_add_style(lv_dropdown_get_list(ui->screen_city_hor_ddlist_3), &style_screen_city_hor_ddlist_3_extra_list_selected_checked, LV_PART_SELECTED | LV_STATE_CHECKED);

    // Write style state: LV_STATE_DEFAULT for &style_screen_city_hor_ddlist_3_extra_list_main_default
    static lv_style_t style_screen_city_hor_ddlist_3_extra_list_main_default;
    ui_init_style(&style_screen_city_hor_ddlist_3_extra_list_main_default);

    lv_style_set_max_height(&style_screen_city_hor_ddlist_3_extra_list_main_default, 235);
    lv_style_set_text_color(&style_screen_city_hor_ddlist_3_extra_list_main_default, lv_color_hex(0x0D3055));
    lv_style_set_text_font(&style_screen_city_hor_ddlist_3_extra_list_main_default, &lv_font_Dengb_24);
    lv_style_set_text_opa(&style_screen_city_hor_ddlist_3_extra_list_main_default, 255);
    lv_style_set_border_width(&style_screen_city_hor_ddlist_3_extra_list_main_default, 0);
    lv_style_set_radius(&style_screen_city_hor_ddlist_3_extra_list_main_default, 3);
    lv_style_set_bg_opa(&style_screen_city_hor_ddlist_3_extra_list_main_default, 255);
    lv_style_set_bg_color(&style_screen_city_hor_ddlist_3_extra_list_main_default, lv_color_hex(0xe7daee));
    lv_style_set_bg_grad_dir(&style_screen_city_hor_ddlist_3_extra_list_main_default, LV_GRAD_DIR_NONE);
    lv_obj_add_style(lv_dropdown_get_list(ui->screen_city_hor_ddlist_3), &style_screen_city_hor_ddlist_3_extra_list_main_default, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write style state: LV_STATE_DEFAULT for &style_screen_city_hor_ddlist_3_extra_list_scrollbar_default
    static lv_style_t style_screen_city_hor_ddlist_3_extra_list_scrollbar_default;
    ui_init_style(&style_screen_city_hor_ddlist_3_extra_list_scrollbar_default);

    lv_style_set_radius(&style_screen_city_hor_ddlist_3_extra_list_scrollbar_default, 0);
    lv_style_set_bg_opa(&style_screen_city_hor_ddlist_3_extra_list_scrollbar_default, 255);
    lv_style_set_bg_color(&style_screen_city_hor_ddlist_3_extra_list_scrollbar_default, lv_color_hex(0x007fff));
    lv_style_set_bg_grad_dir(&style_screen_city_hor_ddlist_3_extra_list_scrollbar_default, LV_GRAD_DIR_NONE);
    lv_obj_add_style(lv_dropdown_get_list(ui->screen_city_hor_ddlist_3), &style_screen_city_hor_ddlist_3_extra_list_scrollbar_default, LV_PART_SCROLLBAR | LV_STATE_DEFAULT);

    // Write codes screen_city_hor_label_1
    ui->screen_city_hor_label_1 = lv_label_create(ui->screen_city_hor);
    lv_label_set_text(ui->screen_city_hor_label_1, "选择城市");
    lv_label_set_long_mode(ui->screen_city_hor_label_1, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->screen_city_hor_label_1, 311, 19);
    lv_obj_set_size(ui->screen_city_hor_label_1, 378, 55);

    // Write style for screen_city_hor_label_1, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_city_hor_label_1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_city_hor_label_1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_city_hor_label_1, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_city_hor_label_1, &lv_font_Dengb_40, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_city_hor_label_1, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_city_hor_label_1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_city_hor_label_1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_city_hor_label_1, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_city_hor_label_1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_city_hor_label_1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_city_hor_label_1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_city_hor_label_1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_city_hor_label_1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_city_hor_label_1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write codes screen_city_hor_btn_city
    ui->screen_city_hor_btn_city = lv_btn_create(ui->screen_city_hor);
    ui->screen_city_hor_btn_city_label = lv_label_create(ui->screen_city_hor_btn_city);
    lv_label_set_text(ui->screen_city_hor_btn_city_label, "OK");
    lv_label_set_long_mode(ui->screen_city_hor_btn_city_label, LV_LABEL_LONG_WRAP);
    lv_obj_align(ui->screen_city_hor_btn_city_label, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_pad_all(ui->screen_city_hor_btn_city, 0, LV_STATE_DEFAULT);
    lv_obj_set_width(ui->screen_city_hor_btn_city_label, LV_PCT(100));
    lv_obj_set_pos(ui->screen_city_hor_btn_city, 890, 1);
    lv_obj_set_size(ui->screen_city_hor_btn_city, 132, 94);

    // Write style for screen_city_hor_btn_city, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_city_hor_btn_city, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_city_hor_btn_city, lv_color_hex(0x3453af), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_city_hor_btn_city, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->screen_city_hor_btn_city, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->screen_city_hor_btn_city, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->screen_city_hor_btn_city, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->screen_city_hor_btn_city, LV_BORDER_SIDE_BOTTOM | LV_BORDER_SIDE_RIGHT, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_city_hor_btn_city, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_city_hor_btn_city, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_city_hor_btn_city, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_city_hor_btn_city, &lv_font_Dengb_24, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_city_hor_btn_city, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_city_hor_btn_city, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);

    // The custom code of screen_city_hor.
    if (MY_SET.language == LANGUAGE_CN)
    {
        lv_label_set_text(ui->screen_city_hor_btn_city_label, "确定");
        lv_label_set_text(ui->screen_city_hor_label_1, "选择城市");
    }
    else
    {
        lv_label_set_text(ui->screen_city_hor_btn_city_label, "OK");
        lv_label_set_text(ui->screen_city_hor_label_1, "SELECT CITY");
    }

    if (MY_SET.language == LANGUAGE_CN)
    {
        lv_dropdown_set_options(ui->screen_city_hor_ddlist_1, city_province);
        lv_dropdown_set_options(ui->screen_city_hor_ddlist_2, my_city_2[my_city_id.city_1].city_name);
    }else{
        lv_dropdown_set_options(ui->screen_city_hor_ddlist_1, city_province_en);
        lv_dropdown_set_options(ui->screen_city_hor_ddlist_2, my_city_2_en[my_city_id.city_1].city_name);
    }

    lv_dropdown_set_selected(guider_ui.screen_city_hor_ddlist_1, my_city_id.city_1);
    lv_dropdown_set_selected(guider_ui.screen_city_hor_ddlist_2, my_city_id.city_2);

    uint16_t city_3_value = 0;
    uint16_t id = lv_dropdown_get_selected(guider_ui.screen_city_hor_ddlist_1);
    uint16_t city_2_value = lv_dropdown_get_selected(guider_ui.screen_city_hor_ddlist_2);
    for (int i = 0; i < id; i++)
        city_3_value += city_cnt[i];
    if (MY_SET.language == LANGUAGE_CN)
    {
        lv_dropdown_set_options(ui->screen_city_hor_ddlist_3, my_city_3[city_3_value + city_2_value].city_name);
    }else
        lv_dropdown_set_options(ui->screen_city_hor_ddlist_3, my_city_3_en[city_3_value + city_2_value].city_name);
    lv_dropdown_set_selected(guider_ui.screen_city_hor_ddlist_3, my_city_id.city_3);

    //
    lv_obj_clear_flag(ui->screen_city_hor, LV_OBJ_FLAG_SCROLLABLE);

    lv_obj_add_event_cb(ui->screen_city_hor_ddlist_1, select_city_handler, LV_EVENT_VALUE_CHANGED, ui);
    lv_obj_add_event_cb(ui->screen_city_hor_ddlist_2, select_city_handler, LV_EVENT_VALUE_CHANGED, ui);
    lv_obj_add_event_cb(ui->screen_city_hor_btn_return, return_handler, LV_EVENT_CLICKED, ui);
    lv_obj_add_event_cb(ui->screen_city_hor_btn_city, ok_handler, LV_EVENT_CLICKED, ui);

    // Update current screen layout.
    lv_obj_update_layout(ui->screen_city_hor);
    my_page = PAGE_CITY_HOR;
}
