// /**
//  * @berief: 设置全段颜色
//  * @param: COLOR_RGB_T rgbData 要设置的颜色
//  * @param: INT_T num 颜色数组的个数
//  * @retval: true 成功
//  */
// OPERATE_LIGHT light_set_rgb_all(COLOR_RGB_T rgbData)
// {
//     OPERATE_LIGHT opt_ret = -1;
        
// 	//PR_DEBUG("rgbData:%d, %d, %d", rgbData.usRed, rgbData.usGreen, rgbData.usBlue);
// 	opt_ret = led_color_set(COLOR_RANGE_CONVERT(rgbData.usRed),COLOR_RANGE_CONVERT(rgbData.usGreen),COLOR_RANGE_CONVERT(rgbData.usBlue));
//     if(opt_ret = LIGHT_OK) {
//         PR_ERR("led_color_reset err !!!");
//         return LIGHT_COM_ERROR;
//     }
//     return LIGHT_OK;
// }

// VOID light_set_color_delay_timerCB(UINT_T timerID,PVOID_T pTimerArg)
// {
// 	OPERATE_LIGHT opt_ret = -1;
// 	opt_ret = light_set_rgb_all(delaySetColor);
//     PR_NOTICE("light_set_color_delay_timerCB");
//     if(opt_ret == LIGHT_OK){
//         opUserSWTimerStop(timerID);
//     }
// }

// OPERATE_RET tuya_add_set_color_delay_sw_timer(VOID)
// {
//     OPERATE_RET opRet = -1;

//     opRet = opUserSWTimerAdd(&SET_COLOR_DELAY, (VOID *)light_set_color_delay_timerCB, NULL);
//     if (opRet != OPRT_OK) {
//         PR_ERR("SET_COLOR_DELAY add error:%d", opRet);
//     }

//     return opRet;
// }

// /**
//  * @berief: 延时设置全段颜色,函数目的是防止直接执行设置颜色时，渐变线程未关闭
//  * @param: COLOR_RGB_T rgbData 要设置的颜色
//  * @param: UINT_T delayMs 延时多长时间
//  * @retval: 
//  */
// OPERATE_LIGHT light_set_rgb_all_delay(COLOR_RGB_T rgbData,UINT_T delayMs)
// {
// 	OPERATE_LIGHT ret;
    
// 	memcpy(&delaySetColor,&rgbData,SIZEOF(COLOR_RGB_T));
// 	opUserSWTimerStart(SET_COLOR_DELAY,delayMs);
//     return LIGHT_OK;
// }

// OPERATE_LIGHT light_send_color_data()
// {
	
// 	return led_data_send();
// }

// //闪烁效果
// VOID light_set_blink_all(COLOR_RGB_T *srcColor,UINT_T colorSum)
// {
// 	STATIC UCHAR_T n = 0;	
// 	STATIC CHAR_T i = 0;
// 	COLOR_RGB_T rgbTmp = {0,0,0};
//     COLOR_RGB_T usColor = {0,0,0};
//     if(get_scene_reset_flag()){
//         i = 0;
//         n = 0;
//         set_scene_reset_flag(FALSE);
//     }
// 	if(i >= colorSum){
// 		i = 0;
// 	}
	
// 	if(n%2 == 0){
//         memcpy(&usColor,&srcColor[i],SIZEOF(usColor));

// 		light_set_rgb_all(usColor);
// 	}
// 	else {
// 		light_set_rgb_all(rgbTmp);
// 	}
// 	if(n % 2 == 1){
// 		i++;
// 	}
// 	n++;
// }

// //贪吃蛇效果
// VOID light_set_snake(COLOR_RGB_T *srcColor,UINT_T colorSum,UINT_T ClockMode)
// {
//     STATIC USHORT_T i = 0;
//     STATIC USHORT_T n = 0;
//     COLOR_RGB_T usColor = {0,0,0};
//     COLOR_RGB_T rstColor = {0,0,0};
//     UINT_T led_cnt;
    
//     led_cnt = get_led_sum();
//     if(get_scene_reset_flag()){
//         i = 0;
//         n = 0;
//         set_scene_reset_flag(FALSE);
//     }
//     i = i % colorSum;     //0 1 2 --- colorSum-1
//     //PR_DEBUG("led_cnt:%d,i:%d,colorSum:%d",led_cnt,i,colorSum);
//     //ClockMode = 1;
//     //放入数据
//     memcpy(&usColor,&srcColor[i],SIZEOF(usColor));//需要限制，特别是白光时
//     //PR_NOTICE("usColor:%d,%d,%d",usColor.usRed,usColor.usGreen,usColor.usBlue);
//     if(n < led_cnt){
//         //注意下面两个函数的调用顺序，先移位再插入和先插入再移位是不一样的，调用顺序不一样，插入函数参数不一样
//         led_shift_cycle_set(ClockMode,0xffff,0xffff,1);
//         if(ClockMode == 0){
//             led_insert_color_set(&usColor,0,1);
//         }else{
//             led_insert_color_set(&usColor,led_cnt-1,1);
//         }
//         light_send_color_data();//亮灯
//     }else{
//         light_set_rgb_all(rstColor);//清空
//     }
//     n++;
//     if(n == led_cnt + 2){//2个定时器时间复位熄灭
//         i++;
//         n=0;
//     }

// }

// //group -->  0x03
// VOID light_group(COLOR_RGB_T *srcColor,UINT_T colorSum,UINT_T ClockMode)
// {
//     STATIC USHORT_T i = 0;
//     STATIC USHORT_T n = 0;
//     USHORT_T k = 0;
//     COLOR_RGB_T usColor = {0,0,0};
//     COLOR_RGB_T rstColor = {0,0,0};
//     UINT_T led_cnt;
    
//     led_cnt = get_led_sum();
//     if(get_scene_reset_flag()){
//         i = 0;
//         n = 0;
//         set_scene_reset_flag(FALSE);
//     }
//     i = i % colorSum;     //0 1 2 --- colorSum-1
//     //PR_DEBUG("led_cnt:%d,i:%d,colorSum:%d",led_cnt,i,colorSum);
//     //ClockMode = 1;
//     //放入数据
//     memcpy(&usColor,&srcColor[i],SIZEOF(usColor));//需要限制，特别是白光时
//     //PR_NOTICE("usColor:%d,%d,%d",usColor.usRed,usColor.usGreen,usColor.usBlue);
//     if(n < led_cnt){
//         //注意下面两个函数的调用顺序，先移位再插入和先插入再移位是不一样的，调用顺序不一样，插入函数参数不一样
//         led_shift_cycle_set(ClockMode,0xffff,0xffff,1);
//         if(ClockMode == 0){
//             led_insert_color_set(&usColor,0,1);
//         }else{
//             led_insert_color_set(&usColor,led_cnt-1,1);
//         }
//         light_send_color_data();//亮灯
//     }else{
//         light_set_rgb_all(rstColor);//清空
//     }
//     n++;
//     if(n == led_cnt){//2个定时器时间复位熄灭
//         n=0;
//     }
//     k = led_cnt/3;
//     if(n % k == (k-1)){
//         i++;
//     }
    
// }


// USHORT_T light_calc_bright_limit(IN USHORT_T usValue)
// {
//     USHORT_T usLimitMin = 10;
//     USHORT_T usLimitMax = 1000;

//     if(usValue < CTRL_RGB_BRIGHT_VALUE_MIN) {
//         return 0;      
//     }
//     usLimitMin = (USHORT_T)(CTRL_RGB_BRIGHT_VALUE_MAX * ( (FLOAT_T)tuya_device_get_hex_config(",colormin:") / 100.0 ));      
//     usLimitMax = (USHORT_T)(CTRL_RGB_BRIGHT_VALUE_MAX * ( (FLOAT_T)tuya_device_get_hex_config(",colormax:") / 100.0 ));

//     return (USHORT_T)((usValue - CTRL_RGB_BRIGHT_VALUE_MIN) * (usLimitMax - usLimitMin) / (CTRL_RGB_BRIGHT_VALUE_MAX - CTRL_RGB_BRIGHT_VALUE_MIN) + usLimitMin);
// }



// const uint8_t BreatheWave[142] = {
// 1,     1,   1,   1,   2,   2,   2,   2,   2,   3, 
// 3,     3,   3,   4,   4,   5,   5,   6,   6,   7, 
// 8,     9,  10,  11,  12,  13,  14,  16,  18,  20,  
// 22,   24,  27,  30,  33,  36,  40,  45,  50,  55, 
// 61,   67,  75,  83,  88,  92,  97, 102, 108, 113, 119, 125, 132, 138, 146, 153, 162,
// 170, 179, 188, 194, 199, 208, 216, 220, 224, 231, 238, 243, 247, 255, 255, 247, 243, 238, 231, 224, 220, 216, 208, 199, 194, 188, 179, 170, 162,
// 153, 146, 138, 132, 125, 119, 113, 108, 102,  97, 92,   88,  83,  75,  67,  61, 
// 55,   50,  45,  40,  36,  33,  30,  27,  24,  22,
// 20,   18,  16,  14,  13,  12,  11,  10,   9,   8,
// 7,     6,   6,   5,   5,   4,   4,   3,   3,   3, 
// 3,     2,   2,   2,   2,   2,   1,   1,   1,   1
// };



// void light_breathe_mode(COLOR_RGB_T *srcColor,UINT_T colorSum)
// {
//     static uint8_t Breathe_Index;
//     static uint8_t Breathe_Cnt;
//     float Breathe_percentage = 0;
// 	static uint8_t color_cnt = 0;
//     COLOR_RGB_T usColor = {0,0,0};
//     //COLOR_RGB_T rstColor = {0,0,0};	

//   	if(Breathe_Index > 141)
// 		Breathe_Index = 0;
//     memcpy(&usColor,&srcColor[color_cnt],SIZEOF(usColor));
//     //PR_DEBUG("usColor:%ld,%ld,%ld\r\n",usColor.usRed,usColor.usGreen,usColor.usBlue);
//     Breathe_percentage = (float)BreatheWave[Breathe_Index]/255;
//     //PR_DEBUG("Breathe_percentage:%f\r\n",Breathe_percentage);

// 	if(Breathe_Cnt < colorSum)
// 	{
// 		usColor.usRed = usColor.usRed*Breathe_percentage;
// 		usColor.usGreen = usColor.usGreen*Breathe_percentage;
// 		usColor.usBlue = usColor.usBlue*Breathe_percentage; 	
// 	}
//     else{
//         Breathe_Cnt = 0;
//         color_cnt = 0;
//     }

// 	if(Breathe_Index < 141)
// 	    Breathe_Index++;
// 	else     //one circle    change color
// 	{
// 	    Breathe_Index = 0;
// 		Breathe_Cnt++;
//         color_cnt++;
// 	}
// 	// rgbData.usRed = (int)origin_rgb_buf[0][0]*1000/255;
// 	// rgbData.usGreen = (int)origin_rgb_buf[0][1]*1000/255;
// 	// rgbData.usBlue = (int)origin_rgb_buf[0][2]*1000/255;
// 	//PR_DEBUG("usColor:%ld,%ld,%ld\r\n",usColor.usRed,usColor.usGreen,usColor.usBlue);
// 	light_set_rgb_all(usColor);		
// }

// VOID light_part_breathe_mode(COLOR_RGB_T *srcColor,UINT_T colorSum,UINT_T ClockMode)
// {
//     uint8_t i = 0;
//     STATIC uint8_t k = 0;
//     STATIC uint8_t n = 0;
//     COLOR_RGB_T usColor = {0,0,0};
//     COLOR_RGB_T dstColor = {0,0,0};
//     static COLOR_RGB_T curColor[8];
//     UINT_T led_cnt;
//     static uint8_t Breathe_Index = 0;
//     static uint8_t Breathe_Cnt;
//     float Breathe_percentage = 0;
// 	static uint8_t color_cnt = 0;
    
//     USHORT_T start_point =0;
//     int step,num_step,last_step = 0;
//     USHORT_T buf_color =0;
//     led_cnt = get_led_sum();
//     if(get_scene_reset_flag()){
//         i = 0;
//         start_point = 0;
//         set_scene_reset_flag(FALSE);
//     }
//     num_step = colorSum;
//     step = led_cnt/num_step;
//     last_step = led_cnt - ((num_step-1)*step);

//     //memcpy(&usColor,&srcColor[color_cnt],SIZEOF(usColor));
//     //PR_DEBUG("usColor:%ld,%ld,%ld\r\n",usColor.usRed,usColor.usGreen,usColor.usBlue);
//     Breathe_percentage = (float)BreatheWave[Breathe_Index]/255;
//     //PR_DEBUG("Breathe_percentage:%f\r\n",Breathe_percentage);

// 	while(i < colorSum)
// 	{
// 		srcColor[i].usRed = srcColor[i].usRed*Breathe_percentage;
// 		srcColor[i].usGreen = srcColor[i].usGreen*Breathe_percentage;
// 		srcColor[i].usBlue = srcColor[i].usBlue*Breathe_percentage; 
//         i++;	
// 	}

// 	if(Breathe_Index < 141)
// 	    Breathe_Index++;
// 	else     //one circle    change color
// 	{
// 	    Breathe_Index = 0;
// 		// Breathe_Cnt++;
//         // color_cnt++;
// 	}

//     i = 0;
//     while(i<colorSum)
//     {
//         if(i<(num_step-1)){
//             led_insert_color_set(&srcColor[i],start_point,step);
//             start_point = start_point + step;
//         }
//         else if(i==(num_step-1))
//         {
//             led_insert_color_set(&srcColor[i],start_point,last_step);
//             start_point = start_point + last_step;           
//         }
//         light_send_color_data();//亮灯
//         //PR_DEBUG("send curColor[%d]:%ld,%ld,%ld\r\n",i,curColor[i].usRed,curColor[i].usGreen,curColor[i].usBlue);
//         i++;
//     }   
//     //PR_DEBUG("light_part_gradual_mode :curColor:%ld,%ld,%ld\r\n",curColor[0].usRed,curColor[0].usGreen,curColor[0].usBlue);
//     //color_ronud++;
// }

// void light_jump_mode(COLOR_RGB_T *srcColor,UINT_T colorSum)
// {
//     USHORT_T i = 0;
//     COLOR_RGB_T usColor = {0,0,0};
//     COLOR_RGB_T rstColor = {0,0,0};
//     UINT_T led_cnt;
//     USHORT_T buf_color =0;
//     STATIC USHORT_T color_ronud = 0;
//     USHORT_T start_point =0;
//     int step,num_step,last_step = 0;
//     led_cnt = get_led_sum();
//     if(get_scene_reset_flag()){
//         i = 0;
//         start_point = 0;
//         set_scene_reset_flag(FALSE);
//     }
//     num_step = colorSum;
//     step = led_cnt/num_step;
//     last_step = led_cnt - ((num_step-1)*step);
    
//     //i = i % colorSum;     //0 1 2 --- colorSum-1
//     //PR_DEBUG("led_cnt:%d,i:%d,colorSum:%d",led_cnt,i,colorSum);
//     //ClockMode = 1;
//     while(i<colorSum)
//     {
//         buf_color = (color_ronud+i)%num_step;
//         // if(buf_color>=num_step)
//         //     buf_color = buf_color - num_step;
//         memcpy(&usColor,&srcColor[buf_color],SIZEOF(usColor));//需要限制，特别是白光时
//         //PR_NOTICE("usColor:%d,%d,%d",usColor.usRed,usColor.usGreen,usColor.usBlue);
//         if(i<(num_step-1)){
//             led_insert_color_set(&usColor,start_point,step);
//             start_point = start_point + step;
//         }
//         else if(i==(num_step-1))
//         {
//             led_insert_color_set(&usColor,start_point,last_step);
//             start_point = start_point + last_step;           
//         }
//         light_send_color_data();//亮灯
//         //light_set_rgb_all(rstColor);//清空
//         i++;
//     }
//     color_ronud++;
// }

// VOID light_float_mode(COLOR_RGB_T *srcColor,UINT_T colorSum,UINT_T ClockMode)
// {
//     STATIC USHORT_T i = 0;
//     STATIC USHORT_T n = 0;
//     USHORT_T k = 0;
//     COLOR_RGB_T usColor = {0,0,0};
//     COLOR_RGB_T rstColor = {0,0,0};
//     UINT_T led_cnt;
//     UINT_T point_cnt = 5;//const
//     led_cnt = get_led_sum();
//     if(get_scene_reset_flag()){
//         i = 0;
//         n = 0;
//         set_scene_reset_flag(FALSE);
//     }
//     i = i % colorSum;     //0 1 2 --- colorSum-1
//     //PR_DEBUG("led_cnt:%d,i:%d,colorSum:%d",led_cnt,i,colorSum);
//     //ClockMode = 1;
//     //放入数据
    
//     //PR_NOTICE("usColor:%d,%d,%d",usColor.usRed,usColor.usGreen,usColor.usBlue);
//     if(n < led_cnt){
//         //注意下面两个函数的调用顺序，先移位再插入和先插入再移位是不一样的，调用顺序不一样，插入函数参数不一样
//         if(n%(point_cnt*2)<point_cnt)
//             memcpy(&usColor,&srcColor[i],SIZEOF(usColor));//需要限制，特别是白光时
//         else
//             memcpy(&usColor,&rstColor,SIZEOF(usColor));//0
//         if(n%(point_cnt*2)==(2*point_cnt-1))
//             i++;

//         led_shift_cycle_set(ClockMode,0xffff,0xffff,1);
//         if(ClockMode == 0){
//             led_insert_color_set(&usColor,0,1);
//         }else{
//             led_insert_color_set(&usColor,led_cnt-1,1);
//         }
//         light_send_color_data();//亮灯
//         n++;
//     }else{
//         n=0;
//         //light_set_rgb_all(rstColor);//清空
//     }
    
// }

// VOID light_meteor_mode(COLOR_RGB_T *srcColor,UINT_T colorSum,UINT_T ClockMode)
// {
//     STATIC USHORT_T i = 0;
//     STATIC USHORT_T n = 0;
//     USHORT_T k = 0;
//     COLOR_RGB_T usColor = {0,0,0};
//     COLOR_RGB_T rstColor = {0,0,0};
//     UINT_T led_cnt;
//     UINT_T point_cnt = 4;//const
//     led_cnt = get_led_sum();
//     if(get_scene_reset_flag()){
//         i = 0;
//         n = 0;
//         set_scene_reset_flag(FALSE);
//     }
//     i = i % colorSum;     //0 1 2 --- colorSum-1
//     //PR_DEBUG("led_cnt:%d,i:%d,colorSum:%d",led_cnt,i,colorSum);
//     //ClockMode = 1;
//     //放入数据
//     memcpy(&usColor,&srcColor[i],SIZEOF(usColor));//需要限制，特别是白光时
//     //PR_NOTICE("usColor:%d,%d,%d",usColor.usRed,usColor.usGreen,usColor.usBlue);
//     if(n < led_cnt){
//         //注意下面两个函数的调用顺序，先移位再插入和先插入再移位是不一样的，调用顺序不一样，插入函数参数不一样
//         if(n<point_cnt){
//             led_shift_cycle_set(ClockMode,0xffff,0xffff,1);
//             if(ClockMode == 0){
//                 led_insert_color_set(&usColor,0,1);
//             }else{
//                 led_insert_color_set(&usColor,led_cnt-1,1);
//             }
//         }           
//         else
//             led_shift_cycle_set(ClockMode,0xffff,0xffff,1);         
//         light_send_color_data();//亮灯
//         n++;
//     }else{
//         n=0;
//         i++;
//         //light_set_rgb_all(rstColor);//清空
//     }
// }

// VOID light_flags_mode(COLOR_RGB_T *srcColor,UINT_T colorSum,UINT_T ClockMode)
// {
//     STATIC USHORT_T i = 0;
//     STATIC USHORT_T k = 0;
//     STATIC USHORT_T n = 0;
//     COLOR_RGB_T usColor = {0,0,0};
//     COLOR_RGB_T dstColor = {0,0,0};
//     COLOR_RGB_T rstColor = {0,0,0};
//     UINT_T led_cnt;
//     UINT_T point_cnt = 5;//const
//     USHORT_T start_point =0;
//     //int step,num_step,last_step = 0;
//     led_cnt = get_led_sum();
//     if(get_scene_reset_flag()){
//         i = 0;
//         n = 0;
//         set_scene_reset_flag(FALSE);
//     }
//     i = i % colorSum;     //0 1 2 --- colorSum-1
//     memcpy(&usColor,&srcColor[i],SIZEOF(usColor));
//     k = i + 1;
//     k = k % colorSum; 
//     memcpy(&dstColor,&srcColor[k],SIZEOF(dstColor));
//     //PR_NOTICE("usColor:%d,%d,%d",usColor.usRed,usColor.usGreen,usColor.usBlue);
//     if(n < (led_cnt/2)){
//         led_insert_color_set(&usColor,n,1);
//         led_insert_color_set(&dstColor,led_cnt-1-n,1);
//         light_send_color_data();//亮灯      
//     }else if(n<led_cnt)
//     {
//         led_insert_color_set(&rstColor,n,1);
//         led_insert_color_set(&rstColor,led_cnt-1-n,1);
//         light_send_color_data();//亮灯   
//     }
//     n++;
//     if(n == led_cnt){
//         n=0;
//         i++;
//     }
// }

// VOID light_gradual_mode(COLOR_RGB_T *srcColor,UINT_T colorSum,UINT_T ClockMode)
// {
//     STATIC uint8_t i = 0;
//     STATIC uint8_t k = 0;
//     STATIC uint8_t n = 0;
// 	static uint8_t color_cnt = 0;
//     COLOR_RGB_T usColor = {0,0,0};
//     COLOR_RGB_T dstColor = {0,0,0};
//     static COLOR_RGB_T curColor = {0,0,0};
//     UINT_T step_cnt = 200;//const
//     //COLOR_RGB_T rstColor = {0,0,0};	
//     UINT_T led_cnt;
//     //float percentage = 0;
//     float delta = 0;
//     led_cnt = get_led_sum();
//     if(get_scene_reset_flag()){
//         i = 0;
//         n = 0;
//         set_scene_reset_flag(FALSE);
//     }
//     i = i % colorSum; 
//     memcpy(&usColor,&srcColor[i],SIZEOF(usColor));
//     k = i + 1;
//     k = k % colorSum; 
//     memcpy(&dstColor,&srcColor[k],SIZEOF(dstColor));

//     if(n==0){
//         memcpy(&curColor,&usColor,SIZEOF(curColor));  //start_value
//         n++;
//     }
//     else if(n < step_cnt){
//         delta = (float)(dstColor.usRed-usColor.usRed)/step_cnt;
//         curColor.usRed = usColor.usRed + delta*n;
//         delta = (float)(dstColor.usGreen-usColor.usGreen)/step_cnt;
//         curColor.usGreen = usColor.usGreen + delta*n;
//         delta = (float)(dstColor.usBlue-usColor.usBlue)/step_cnt;
//         curColor.usBlue = usColor.usBlue + delta*n;
//         n++;
//     }
//     else if(n == step_cnt){
//         //memcpy(&curColor,&dstColor,SIZEOF(curColor)); //end_value
//         i++;
//         n=0;
//     }
//     //light_send_color_data();//亮灯
// 	//PR_DEBUG("light_gradual_mode :curColor:%ld,%ld,%ld\r\n",curColor.usRed,curColor.usGreen,curColor.usBlue);
// 	light_set_rgb_all(curColor);	
// }


// //COLOR_RGB_T curColor[8];
// //USHORT_T color_ronud2 = 0;
// VOID light_part_gradual_mode(COLOR_RGB_T *srcColor,UINT_T colorSum,UINT_T ClockMode)
// {
//     uint8_t i = 0;
//     uint8_t k = 0;
//     STATIC uint8_t n = 0;
// 	//static uint8_t color_cnt = 0;
//     STATIC uint8_t color_ronud_2 = 0;   //stack spill
//     COLOR_RGB_T usColor = {0,0,0};
//     COLOR_RGB_T dstColor = {0,0,0};
//     static COLOR_RGB_T curColor[8];
//     UINT_T step_cnt = 200;//const	
//     UINT_T led_cnt;
//     float delta = 0;
    
//     USHORT_T start_point =0;
//     int step,num_step,last_step = 0;
//     USHORT_T buf_color =0;
//     led_cnt = get_led_sum();
//     if(get_scene_reset_flag()){
//         i = 0;
//         start_point = 0;
//         set_scene_reset_flag(FALSE);
//     }
//     num_step = colorSum;
//     step = led_cnt/num_step;
//     last_step = led_cnt - ((num_step-1)*step);

//     if(n==0){
//         i = 0;
//         while(i<colorSum){
//             buf_color = (color_ronud_2+i)%colorSum;
//             memcpy(&curColor[i],&srcColor[buf_color],SIZEOF(curColor));  //start_value
//              PR_DEBUG("start curColor[%d]:%ld,%ld,%ld\r\n",i,curColor[i].usRed,curColor[i].usGreen,curColor[i].usBlue);
//              PR_DEBUG("buf_color,color_ronud_2:%d,%d\r\n",buf_color,color_ronud_2);
//             i++;
//         }
//         n++;
//     }
//     else if(n < step_cnt){
//         i = 0;
//         while(i<colorSum){
//             buf_color = (color_ronud_2+i)%colorSum;
//             memcpy(&usColor,&srcColor[buf_color],SIZEOF(usColor));
//             k = buf_color + 1;
//             k = k % colorSum; 
//             memcpy(&dstColor,&srcColor[k],SIZEOF(dstColor));
//             //PR_DEBUG("buf_color,color_ronud:%d,%d\r\n",buf_color,color_ronud);
//             //PR_DEBUG("usColor:%ld,%ld,%ld\r\n",usColor.usRed,usColor.usGreen,usColor.usBlue);
//             delta = (float)(dstColor.usRed-usColor.usRed)/step_cnt;
//             curColor[i].usRed = usColor.usRed + delta*n;
//             delta = (float)(dstColor.usGreen-usColor.usGreen)/step_cnt;
//             curColor[i].usGreen = usColor.usGreen + delta*n;
//             delta = (float)(dstColor.usBlue-usColor.usBlue)/step_cnt;
//             curColor[i].usBlue = usColor.usBlue + delta*n;
//             //PR_DEBUG("output curColor[%d]:%ld,%ld,%ld\r\n",i,curColor[i].usRed,curColor[i].usGreen,curColor[i].usBlue);
//             i++;     
//         }
//         n++;
//     }
//     else if(n == step_cnt){
//         color_ronud_2++;
//         n=0;
//     }
//     i = 0;
//     while(i<colorSum)
//     {
//         if(i<(num_step-1)){
//             led_insert_color_set(&curColor[i],start_point,step);
//             start_point = start_point + step;
//         }
//         else if(i==(num_step-1))
//         {
//             led_insert_color_set(&curColor[i],start_point,last_step);
//             start_point = start_point + last_step;           
//         }
//         light_send_color_data();//亮灯
//         //PR_DEBUG("send curColor[%d]:%ld,%ld,%ld\r\n",i,curColor[i].usRed,curColor[i].usGreen,curColor[i].usBlue);
//         i++;
//     }   
//     //PR_DEBUG("light_part_gradual_mode :curColor:%ld,%ld,%ld\r\n",curColor[0].usRed,curColor[0].usGreen,curColor[0].usBlue);
// }

// VOID light_rainbow_mode(COLOR_RGB_T *srcColor,UINT_T colorSum,UINT_T ClockMode)
// {
//     STATIC USHORT_T i = 0;
//     STATIC USHORT_T n = 0;
//     STATIC USHORT_T buf_round = 0;
//     //STATIC uint8_t color_ronud3 = 0;
//     USHORT_T k = 0;
//     COLOR_RGB_T usColor = {0,0,0};
//     COLOR_RGB_T dstColor = {0,0,0};
//     STATIC COLOR_RGB_T curColor = {0,0,0};
//     COLOR_RGB_T rstColor = {0,0,0};
//     UINT_T led_cnt;
//     //UINT_T point_cnt = 4;//const
//     USHORT_T buf_color =0;
//     UINT_T step_cnt = 50;//const	
//     float delta = 0;
//     led_cnt = get_led_sum();
//     if(get_scene_reset_flag()){
//         i = 0;
//         buf_round = 0;
//         set_scene_reset_flag(FALSE);
//     }

//     if(buf_round==0){
//         buf_color = color_ronud3%colorSum;
//         memcpy(&curColor,&srcColor[buf_color],SIZEOF(curColor));  //start_value
//         // PR_DEBUG("start curColor:%ld,%ld,%ld\r\n",curColor.usRed,curColor.usGreen,curColor.usBlue);
//         // PR_DEBUG("buf_color,color_ronud3:%d,%d\r\n",buf_color,color_ronud3);
//         buf_round++;
//     }
//     else if(buf_round < led_cnt){
//         buf_color = color_ronud3%colorSum;
//         memcpy(&usColor,&srcColor[buf_color],SIZEOF(usColor));
//         k = buf_color + 1;
//         k = k % colorSum; 
//         memcpy(&dstColor,&srcColor[k],SIZEOF(dstColor));
//         // PR_DEBUG("buf_color,color_ronud3:%d,%d\r\n",buf_color,color_ronud3);
//         // PR_DEBUG("usColor:%ld,%ld,%ld\r\n",usColor.usRed,usColor.usGreen,usColor.usBlue);
//         // PR_DEBUG("dstColor:%ld,%ld,%ld\r\n",dstColor.usRed,dstColor.usGreen,dstColor.usBlue);
//         delta = (float)(dstColor.usRed-usColor.usRed)/led_cnt;
//         curColor.usRed = usColor.usRed + delta*buf_round;
//         delta = (float)(dstColor.usGreen-usColor.usGreen)/led_cnt;
//         curColor.usGreen = usColor.usGreen + delta*buf_round;
//         delta = (float)(dstColor.usBlue-usColor.usBlue)/led_cnt;
//         curColor.usBlue = usColor.usBlue + delta*buf_round;
//         //PR_DEBUG("output curColor:%ld,%ld,%ld\r\n",curColor.usRed,curColor.usGreen,curColor.usBlue);
//         buf_round++;
//     }
//     else if(buf_round == led_cnt){
//         color_ronud3++;
//         buf_round=0;
//     }

//     //i = i % colorSum;     //0 1 2 --- colorSum-1
//     //PR_DEBUG("led_cnt:%d,i:%d,colorSum:%d",led_cnt,i,colorSum);
//     //ClockMode = 1;
//     //放入数据
//     //memcpy(&usColor,&srcColor[i],SIZEOF(usColor));//需要限制，特别是白光时
//     //PR_NOTICE("usColor:%d,%d,%d",usColor.usRed,usColor.usGreen,usColor.usBlue);
//     if(n < led_cnt){
//         //注意下面两个函数的调用顺序，先移位再插入和先插入再移位是不一样的，调用顺序不一样，插入函数参数不一样
//         led_shift_cycle_set(ClockMode,0xffff,0xffff,1);
//         if(ClockMode == 0){
//             led_insert_color_set(&curColor,0,1);
//         }else{
//             led_insert_color_set(&curColor,led_cnt-1,1);
//         }
//         light_send_color_data();//亮灯
//         n++;
//     }
//     else{
//         //light_set_rgb_all(rstColor);//清空
//         n=0;
//     }
    
//     // if(n == led_cnt){//2个定时器时间复位熄灭
//     //     n=0;
//     // }
//     //k = led_cnt/3;
//     // if(n % k == (k-1)){
//     //     i++;
//     // }
// }