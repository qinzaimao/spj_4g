#include "cfgsave.h"

volatile uint8_t save_num = 0;
struct elevator set = {0};

void cfgSave()
{
    FILE *cfgfile;
    char buff[save_byte] = {0x55, 0xAA, 0, 0, 0, 0, 0, 0};
    cfgfile = fopen("/data/config.bin", "wb");
    if (cfgfile == NULL)
    {
        return;
    }
    // rt_kprintf("config save.....\n");
    buff[2] = MY_SET.sound;
    buff[3] = MY_SET.backlight;
    buff[4] = video_defuat_flag;
    buff[5] = voice_language;
    buff[6] = image_have_cnt;
    buff[7] = (MY_SET_TIME.year % 2000);
    buff[8] = MY_SET_TIME.month;
    buff[9] = MY_SET_TIME.day;
    buff[10] = MY_SET_TIME.hour;
    buff[11] = MY_SET_TIME.minute;
    buff[12] = MY_SET_TIME.second;
    buff[13] = MY_SET.language;
    buff[14] = MY_SET.e_con_backlight;
    buff[15] = day_reset_time;
    buff[16] = reset_time_flag;
    buff[17] = password / 100;
    buff[18] = password % 100;
    buff[19] = MY_SET_DHCP.dhcp_state;
    buff[20] = IO_value[0];
    buff[21] = IO_value[1];
    buff[22] = IO_value[2];
    buff[23] = IO_value[3];
    buff[24] = IO_value[4];
    buff[25] = IO_value[5];
    buff[26] = IO_value[6];
    buff[27] = IO_value[7];
    buff[28] = IO_value[8];
    buff[29] = IO_value[9];
    buff[30] = IO_value[10];
    buff[31] = IO_value[11];
    buff[32] = IO_value[12];
    buff[33] = IO_value[13];
    buff[34] = IO_value[14];
    buff[35] = IO_value[15];
    buff[36] = IO_value[16];
    buff[37] = IO_value[17];
    buff[38] = IO_value[18];
    buff[39] = IO_value[19];
    buff[40] = IO_value[20];
    buff[41] = IO_value[21];
    buff[42] = IO_value[22];
    buff[43] = IO_value[23];
    buff[44] = IO_value[24];
    buff[45] = IO_value[25];
    buff[46] = IO_value[26];
    buff[47] = IO_value[27];
    buff[48] = IO_value[28];
    buff[49] = IO_value[29];
    buff[50] = IO_value[30];
    buff[51] = IO_value[31];
    buff[52] = IO_value[32];
    buff[53] = IO_value[33];
    buff[54] = IO_value[34];
    buff[55] = IO_value[35];
    buff[56] = IO_value[36];
    buff[57] = IO_value[37];

    buff[58] = MY_SET.play_mode;
    buff[59] = MY_SET_IMAGE.image;
    buff[60] = MY_SET_IMAGE.logo;
    buff[61] = MY_SET_IMAGE.arrow;
    buff[62] = MY_VOICE_SWITCH.voice;
    buff[63] = MY_VOICE_SWITCH.lr;
    buff[64] = MY_VOICE_SWITCH.block;
    buff[65] = MY_VOICE_SWITCH.dzz;
    buff[66] = MY_VOICE_SWITCH.floor;
    buff[67] = MY_VOICE_SWITCH.fire;
    buff[68] = MY_VOICE_SWITCH.door;
    buff[69] = MY_VOICE_SWITCH.peak;
    buff[70] = MY_VOICE_SWITCH.ols;
    buff[71] = MY_VOICE_SWITCH.up;
    buff[72] = MY_VOICE_SWITCH.appease;

    buff[73] = music_state[0];
    buff[74] = music_state[1];
    buff[75] = music_state[2];

    buff[76] = work_start_time;
    buff[77] = work_over_time;


    buff[79] =  MY_SET_DHCP.ip[0];
    buff[80] =  MY_SET_DHCP.ip[1];
    buff[81] =  MY_SET_DHCP.ip[2];
    buff[82] =  MY_SET_DHCP.ip[3];

    buff[83] =  MY_SET_DHCP.mask[0];
    buff[84] =  MY_SET_DHCP.mask[1];
    buff[85] =  MY_SET_DHCP.mask[2];
    buff[86] =  MY_SET_DHCP.mask[3];

    buff[87] =  MY_SET_DHCP.gateway[0];
    buff[88] =  MY_SET_DHCP.gateway[1];
    buff[89] =  MY_SET_DHCP.gateway[2];
    buff[90] =  MY_SET_DHCP.gateway[3];

    buff[91] =  MY_SET_DHCP.dns[0];
    buff[92] =  MY_SET_DHCP.dns[1];
    buff[93] =  MY_SET_DHCP.dns[2];
    buff[94] =  MY_SET_DHCP.dns[3];

    buff[95] = my_city_id.city_1;
    buff[96] = my_city_id.city_2;
    buff[97] = my_city_id.city_3;
    buff[98] = (char)(my_city_id.city_value & 0xFF);
    buff[99] = (char)((my_city_id.city_value >> 8) & 0xFF);

    buff[100] = city2_temp;
   // 拆分并存储到buff数组
    buff[101] = (char)(city3_temp & 0xFF);  // 保存低8位
    buff[102] = (char)((city3_temp >> 8) & 0xFF);  // 保存高8位

    buff[103] = page_image_cnt[0];
    buff[104] = page_image_cnt[1];
    buff[105] = page_image_cnt[3];
    buff[106] = page_image_cnt[4];
    buff[107] = page_image_cnt[5];
    buff[108] = page_image_cnt[6];
    buff[109] = page_image_cnt[7];
    buff[110] = page_image_cnt[8];
    buff[111] = page_image_cnt[9];
    buff[112] = page_image_cnt[10];
    buff[113] = page_mp4_update_flag[0][0];
    buff[114] = page_mp4_update_flag[0][1];
    buff[115] = page_mp4_update_flag[0][3];
    buff[116] = page_mp4_update_flag[0][4];
    buff[117] = page_mp4_update_flag[0][5];
    buff[118] = page_mp4_update_flag[0][6];
    buff[119] = page_mp4_update_flag[0][7];
    buff[120] = page_mp4_update_flag[0][8];
    buff[121] = page_mp4_update_flag[0][9];
    buff[122] = page_mp4_update_flag[0][10];
    buff[123] = have_txt_flag[0];
    buff[124] = have_txt_flag[1];
    buff[125] = have_txt_flag[2];
    buff[126] = have_txt_flag[3];
    buff[127] = have_txt_flag[4];
    buff[128] = have_txt_flag[5];
    buff[129] = have_txt_flag[6];
    buff[130] = have_txt_flag[7];
    buff[131] = have_txt_flag[8];
    buff[132] = have_txt_flag[9];
    buff[133] = have_txt_flag[10];
    buff[134] = have_txt_flag[11];
    buff[135] = have_txt_flag[12];
    buff[136] = have_txt_flag[13];

    buff[137] = txt_mode[0];
    buff[138] = txt_mode[1];
    buff[139] = txt_mode[2];
    buff[140] = txt_mode[3];
    buff[141] = txt_mode[4];
    buff[142] = txt_mode[5];
    buff[143] = txt_mode[6];
    buff[144] = txt_mode[7];
    buff[145] = txt_mode[8];
    buff[146] = txt_mode[9];
    buff[147] = txt_mode[10];
    buff[148] = txt_mode[11];
    buff[149] = txt_mode[12];
    buff[150] = txt_mode[13];

    buff[151] = page_play_mode[0];
    buff[152] = page_play_mode[1];
    buff[153] = page_play_mode[2];
    buff[154] = page_play_mode[3];
    buff[155] = page_play_mode[4];
    buff[156] = page_play_mode[5];
    buff[157] = page_play_mode[6];
    buff[158] = page_play_mode[7];
    buff[159] = page_play_mode[8];
    buff[160] = page_play_mode[9];
    buff[161] = page_play_mode[10];
    buff[162] = page_play_mode[11];
    buff[163] = page_play_mode[12];
    buff[164] = page_play_mode[13];

    buff[165] = page_avi_update_flag[0][0];
    buff[166] = page_avi_update_flag[0][1];
    buff[167] = page_avi_update_flag[0][3];
    buff[168] = page_avi_update_flag[0][4];
    buff[169] = page_avi_update_flag[0][5];
    buff[170] = page_avi_update_flag[0][6];
    buff[171] = page_avi_update_flag[0][7];
    buff[172] = page_avi_update_flag[0][8];
    buff[173] = page_avi_update_flag[0][9];
    buff[174] = page_avi_update_flag[0][10];

    buff[175] = page_avi_update_flag[1][0];
    buff[176] = page_avi_update_flag[1][1];
    buff[177] = page_avi_update_flag[1][3];
    buff[178] = page_avi_update_flag[1][4];
    buff[179] = page_avi_update_flag[1][5];
    buff[180] = page_avi_update_flag[1][6];
    buff[181] = page_avi_update_flag[1][7];
    buff[182] = page_avi_update_flag[1][8];
    buff[183] = page_avi_update_flag[1][9];
    buff[184] = page_avi_update_flag[1][10];

    buff[185] = page_avi_update_flag[2][0];
    buff[186] = page_avi_update_flag[2][1];
    buff[187] = page_avi_update_flag[2][3];
    buff[188] = page_avi_update_flag[2][4];
    buff[189] = page_avi_update_flag[2][5];
    buff[190] = page_avi_update_flag[2][6];
    buff[191] = page_avi_update_flag[2][7];
    buff[192] = page_avi_update_flag[2][8];
    buff[193] = page_avi_update_flag[2][9];
    buff[194] = page_avi_update_flag[2][10];

    buff[195] = page_mp4_update_flag[1][0];
    buff[196] = page_mp4_update_flag[1][1];
    buff[197] = page_mp4_update_flag[1][3];
    buff[198] = page_mp4_update_flag[1][4];
    buff[199] = page_mp4_update_flag[1][5];
    buff[200] = page_mp4_update_flag[1][6];
    buff[201] = page_mp4_update_flag[1][7];
    buff[202] = page_mp4_update_flag[1][8];
    buff[203] = page_mp4_update_flag[1][9];
    buff[204] = page_mp4_update_flag[1][10];

    buff[205] = page_mp4_update_flag[2][0];
    buff[206] = page_mp4_update_flag[2][1];
    buff[207] = page_mp4_update_flag[2][3];
    buff[208] = page_mp4_update_flag[2][4];
    buff[209] = page_mp4_update_flag[2][5];
    buff[210] = page_mp4_update_flag[2][6];
    buff[211] = page_mp4_update_flag[2][7];
    buff[212] = page_mp4_update_flag[2][8];
    buff[213] = page_mp4_update_flag[2][9];
    buff[214] = page_mp4_update_flag[2][10];
    buff[215] = save_img_page;
    // buff[103] = my_weather.temperature;
    // buff[104] = (char)(my_weather.weather & 0xFF);  // 保存低8位
    // buff[105] = (char)((my_weather.weather >> 8) & 0xFF);  // 保存高8位

    fwrite(buff, 2, save_byte, cfgfile);
    fclose(cfgfile);
    rt_kprintf("config save ok\n");
}

void cfgRead()
{
    FILE *cfgfile;
    char buff[save_byte];
    size_t bytes_read;

    cfgfile = fopen("/data/config.bin", "rb");
    if (cfgfile == NULL)
    {
        rt_kprintf("no config found, using defaults\n");
        return;
    }

    // 修复：正确调用fread，读取16个字节
    bytes_read = fread(buff, 1, save_byte, cfgfile);
    fclose(cfgfile);

    rt_kprintf(">>>>>>>>>>%02x %02x %02x\n", buff[0] & 0xFF, buff[1] & 0xFF, buff[2] & 0xFF);

    // 验证读取的字节数和魔数
    if (bytes_read == save_byte && (buff[0] == 0x55) && (buff[1] == 0xAA))
    {
        // 从文件加载配置
        MY_SET.sound = buff[2];
        MY_SET.backlight = buff[3];
        video_defuat_flag = buff[4];
        voice_language = buff[5];
        image_have_cnt = buff[6];
        MY_SET_TIME.year = buff[7] + 2000;
        MY_SET_TIME.month = buff[8];
        MY_SET_TIME.day = buff[9];
        MY_SET_TIME.hour = buff[10];
        MY_SET_TIME.minute = buff[11];
        MY_SET_TIME.second = buff[12];
        MY_SET.language = buff[13];
        MY_SET.e_con_backlight = buff[14];
        day_reset_time = buff[15];
        reset_time_flag = buff[16];
        password = buff[17] * 100 + buff[18];

        MY_SET_DHCP.dhcp_state = buff[19];
        IO_value[0] = buff[20];
        IO_value[1] = buff[21];
        IO_value[2] = buff[22];
        IO_value[3] = buff[23];
        IO_value[4] = buff[24];
        IO_value[5] = buff[25];
        IO_value[6] = buff[26];
        IO_value[7] = buff[27];
        IO_value[8] = buff[28];
        IO_value[9] = buff[29];
        IO_value[10] = buff[30];
        IO_value[11] = buff[31];
        IO_value[12] = buff[32];
        IO_value[13] = buff[33];
        IO_value[14] = buff[34];
        IO_value[15] = buff[35];
        IO_value[16] = buff[36];
        IO_value[17] = buff[37];
        IO_value[18] = buff[38];
        IO_value[19] = buff[39];
        IO_value[20] = buff[40];
        IO_value[21] = buff[41];
        IO_value[22] = buff[42];
        IO_value[23] = buff[43];
        IO_value[24] = buff[44];
        IO_value[25] = buff[45];
        IO_value[26] = buff[46];
        IO_value[27] = buff[47];
        IO_value[28] = buff[48];
        IO_value[29] = buff[49];
        IO_value[30] = buff[50];
        IO_value[31] = buff[51];
        IO_value[32] = buff[52];
        IO_value[33] = buff[53];
        IO_value[34] = buff[54];
        IO_value[35] = buff[55];
        IO_value[36] = buff[56];
        IO_value[37] = buff[57];

        MY_SET.play_mode = buff[58];
        MY_SET_IMAGE.image = buff[59];
        MY_SET_IMAGE.logo = buff[60];
        MY_SET_IMAGE.arrow = buff[61];

        MY_VOICE_SWITCH.voice = buff[62];
        MY_VOICE_SWITCH.lr = buff[63];
        MY_VOICE_SWITCH.block = buff[64];
        MY_VOICE_SWITCH.dzz = buff[65];
        MY_VOICE_SWITCH.floor = buff[66];
        MY_VOICE_SWITCH.fire = buff[67];
        MY_VOICE_SWITCH.door = buff[68];
        MY_VOICE_SWITCH.peak = buff[69];
        MY_VOICE_SWITCH.ols = buff[70];
        MY_VOICE_SWITCH.up = buff[71];
        MY_VOICE_SWITCH.appease = buff[72];

        music_state[0] = buff[73];
        music_state[1] = buff[74];
        music_state[2] = buff[75];

        work_start_time = buff[76];
        work_over_time = buff[77];

        MY_SET_DHCP.ip[0] = buff[79];
        MY_SET_DHCP.ip[1] = buff[80];
        MY_SET_DHCP.ip[2] = buff[81];
        MY_SET_DHCP.ip[3] = buff[82];

        MY_SET_DHCP.mask[0] = buff[83];
        MY_SET_DHCP.mask[1] = buff[84];
        MY_SET_DHCP.mask[2] = buff[85];
        MY_SET_DHCP.mask[3] = buff[86];

        MY_SET_DHCP.gateway[0] = buff[87];
        MY_SET_DHCP.gateway[1] = buff[88];
        MY_SET_DHCP.gateway[2] = buff[89];
        MY_SET_DHCP.gateway[3] = buff[90];

        MY_SET_DHCP.dns[0] = buff[91];
        MY_SET_DHCP.dns[1] = buff[92];
        MY_SET_DHCP.dns[2] = buff[93];
        MY_SET_DHCP.dns[3] = buff[94];

        my_city_id.city_1 = buff[95];
        my_city_id.city_2 = buff[96];
        my_city_id.city_3 = buff[97];
        my_city_id.city_value = ((uint16_t)buff[99] << 8) | (uint16_t)buff[98];
        city2_temp = buff[100];
        city3_temp = ((uint16_t)buff[102] << 8) | (uint16_t)buff[101];
        page_image_cnt[0] = buff[103];
        page_image_cnt[1] = buff[104];
        page_image_cnt[3] = buff[105];
        page_image_cnt[4] = buff[106];
        page_image_cnt[5] = buff[107];
        page_image_cnt[6] = buff[108];
        page_image_cnt[7] = buff[109];
        page_image_cnt[8] = buff[110];
        page_image_cnt[9] = buff[111];
        page_image_cnt[10] = buff[112];
        page_mp4_update_flag[0][0] = buff[113];
        page_mp4_update_flag[0][1] = buff[114];
        page_mp4_update_flag[0][3] = buff[115];
        page_mp4_update_flag[0][4] = buff[116];
        page_mp4_update_flag[0][5] = buff[117];
        page_mp4_update_flag[0][6] = buff[118];
        page_mp4_update_flag[0][7] = buff[119];
        page_mp4_update_flag[0][8] = buff[120];
        page_mp4_update_flag[0][9] = buff[121];
        page_mp4_update_flag[0][10] = buff[122];
        have_txt_flag[0] = buff[123];
        have_txt_flag[1] = buff[124];
        have_txt_flag[2] = buff[125];
        have_txt_flag[3] = buff[126];
        have_txt_flag[4] = buff[127];
        have_txt_flag[5] = buff[128];
        have_txt_flag[6] = buff[129];
        have_txt_flag[7] = buff[130];
        have_txt_flag[8] = buff[131];
        have_txt_flag[9] = buff[132];
        have_txt_flag[10] = buff[133];
        have_txt_flag[11] = buff[134];
        have_txt_flag[12] = buff[135];
        have_txt_flag[13] = buff[136];

        txt_mode[0] = buff[137];
        txt_mode[1] = buff[138];
        txt_mode[2] = buff[139];
        txt_mode[3] = buff[140];
        txt_mode[4] = buff[141];
        txt_mode[5] = buff[142];
        txt_mode[6] = buff[143];
        txt_mode[7] = buff[144];
        txt_mode[8] = buff[145];
        txt_mode[9] = buff[146];
        txt_mode[10] = buff[147];
        txt_mode[11] = buff[148];
        txt_mode[12] = buff[149];
        txt_mode[13] = buff[150];

        page_play_mode[0] = buff[151];
        page_play_mode[1] = buff[152];
        page_play_mode[2] = buff[153];
        page_play_mode[3] = buff[154];
        page_play_mode[4] = buff[155];
        page_play_mode[5] = buff[156];
        page_play_mode[6] = buff[157];
        page_play_mode[7] = buff[158];
        page_play_mode[8] = buff[159];
        page_play_mode[9] = buff[160];
        page_play_mode[10] = buff[161];
        page_play_mode[11] = buff[162];
        page_play_mode[12] = buff[163];
        page_play_mode[13] = buff[164];

        page_avi_update_flag[0][0] = buff[165];
        page_avi_update_flag[0][1] = buff[166];
        page_avi_update_flag[0][3] = buff[167];
        page_avi_update_flag[0][4] = buff[168];
        page_avi_update_flag[0][5] = buff[169];
        page_avi_update_flag[0][6] = buff[170];
        page_avi_update_flag[0][7] = buff[171];
        page_avi_update_flag[0][8] = buff[172];
        page_avi_update_flag[0][9] = buff[173];
        page_avi_update_flag[0][10] = buff[174];
        page_avi_update_flag[1][0] = buff[175];
        page_avi_update_flag[1][1] = buff[176];
        page_avi_update_flag[1][3] = buff[177];
        page_avi_update_flag[1][4] = buff[178];
        page_avi_update_flag[1][5] = buff[179];
        page_avi_update_flag[1][6] = buff[180];
        page_avi_update_flag[1][7] = buff[181];
        page_avi_update_flag[1][8] = buff[182];
        page_avi_update_flag[1][9] = buff[183];
        page_avi_update_flag[1][10] = buff[184];
        page_avi_update_flag[2][0] = buff[185];
        page_avi_update_flag[2][1] = buff[186];
        page_avi_update_flag[2][3] = buff[187];
        page_avi_update_flag[2][4] = buff[188];
        page_avi_update_flag[2][5] = buff[189];
        page_avi_update_flag[2][6] = buff[190];
        page_avi_update_flag[2][7] = buff[191];
        page_avi_update_flag[2][8] = buff[192];
        page_avi_update_flag[2][9] = buff[193];
        page_avi_update_flag[2][10] = buff[194];


        page_mp4_update_flag[1][0] = buff[195];
        page_mp4_update_flag[1][1] = buff[196];
        page_mp4_update_flag[1][3] = buff[197];
        page_mp4_update_flag[1][4] = buff[198];
        page_mp4_update_flag[1][5] = buff[199];
        page_mp4_update_flag[1][6] = buff[200];
        page_mp4_update_flag[1][7] = buff[201];
        page_mp4_update_flag[1][8] = buff[202];
        page_mp4_update_flag[1][9] = buff[203];
        page_mp4_update_flag[1][10] = buff[204];

        page_mp4_update_flag[2][0] = buff[205];
        page_mp4_update_flag[2][1] = buff[206];
        page_mp4_update_flag[2][3] = buff[207];
        page_mp4_update_flag[2][4] = buff[208];
        page_mp4_update_flag[2][5] = buff[209];
        page_mp4_update_flag[2][6] = buff[210];
        page_mp4_update_flag[2][7] = buff[211];
        page_mp4_update_flag[2][8] = buff[212];
        page_mp4_update_flag[2][9] = buff[213];
        page_mp4_update_flag[2][10] = buff[214];
        save_img_page = buff[215];
        // my_weather.temperature = buff[103];
        // my_weather.temperature = buff[103];
        // my_weather.weather = ((uint16_t)buff[105] << 8) | (uint16_t)buff[104];


        rt_kprintf("year:%d,month:%d,day:%d,hour:%d,minute:%d,second:%d\n",
                    buff[7] + 2000, buff[8], buff[9], buff[10], buff[11], buff[12]);
        rt_kprintf("Config loaded from file\n");
    }
}

void save_begin(void)
{
    save_flag = true;
    save_num++;
}

void cfgsave_thread_entry(void *parameter)
{
    static uint8_t save_counter = 0, last_save_num = 0;

    while (1)
    {
        if (save_flag)
        {
            if ((++save_counter) >= 1 && last_save_num == save_num)
            {
                cfgSave();
                save_num = 0;
                save_counter = 0;
                save_flag = false;
            }
            else if (last_save_num != save_num)
            {
                save_counter = 0;
                last_save_num = save_num;
            }
        }

        // if(need_to_play_next_video)
        // {
        //     rt_tick_t now_time = rt_tick_get();
        //     if (now_time - video_wait_time > 8000)
        //     {
        //         rt_kprintf("多个视频时需要销毁超过8秒,重启视频线程\n");
        //         restart_video_thread();
        //     }
        // }

        rt_thread_mdelay(500);
    }
}
