//===========================================================================
//
//  版权所有者： 刘新国，浙江大学计算机科学与技术学院
//                       CAD&CG国家重点实验室
//               杨帆 3200104595 
//               
//  最近修改：2021年6月7日 
//  初次创建：2020年3月21日
//
//===========================================================================

/**************************************************************
原功能： 
1. 如何使用button
2. 如何输入封闭多边形
3. 如何限定多边形输入的区域，避免和button相互干扰


实现功能：
1. 判断选中返回0or1； 
2. 每一个button有自己的特殊编号，便于区别； 
***************************************************************/
int themebutton(int id, double x, double y, double w, double h, char *label);

// 定义一个矩形区域，输入多边形
//  防止和button的输入干扰
double canvasRect[4] = {0.5, 5.5, 0.5, 5.5};

int InCanvasRect(double x, double y)
{
	return (x >= canvasRect[0] && x <= canvasRect[1] &&
			y >= canvasRect[2] && y <= canvasRect[3] );
}


void SetCanvasRect()
{
	double w = GetWindowWidth();
	double h = GetWindowHeight();
	canvasRect[0] = 0.5;
	canvasRect[1] = w-0.5;
	canvasRect[2] = 1.0;
	canvasRect[3] = h - 0.5;
}

