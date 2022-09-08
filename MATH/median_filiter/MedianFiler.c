/*
 * @Author: 奶茶不加冰
 * @Date: 2022-07-07 10:13:13
 * @LastEditors: OBKoro1
 * @LastEditTime: 2022-07-19 17:02:18
 * @FilePath: \USERf:\资料整理\STM32F4\stm32f407ze_Prj\Template工程模板_V2.0\MATH\median_filiter\MedianFiler.c
 * @Description: 
 */
#include "MedianFiler.h"

float Median_filter(int data, int measureNum, int *Filterdata)
{
	unsigned int i = 0;
	unsigned int j = 0;
	int temp;
	unsigned int MAX_error_targe = 0;
	int MAX_error1;
	Filterdata[measureNum - 1] = data;
	for (i = 0; i < measureNum - 1; i++)
	{
		Filterdata[i] = Filterdata[i + 1];
	}
	for (i = 0; i < measureNum - 1; i++)
	{
		for (j = 0; j < measureNum - 1 - i; j++)
		{
			if (Filterdata[j] > Filterdata[j + 1])
			{
				temp = Filterdata[j];
				Filterdata[j] = Filterdata[j + 1];
				Filterdata[j + 1] = temp;
			}
		}
	}
	MAX_error1 = Filterdata[1] - Filterdata[0];
	for (i = 1; i < measureNum - 1; i++)
	{
		if (MAX_error1 < Filterdata[i + 1] - Filterdata[i])
		{
			MAX_error1 = Filterdata[i + 1] - Filterdata[i];
			MAX_error_targe = i;
		}
	}
	float Average_data = 0;
	if (MAX_error_targe + 1 > (measureNum + 1) / 2)
	{
		for (i = 0; i <= MAX_error_targe; i++)
		{
			Average_data += Filterdata[i];
		}
		Average_data /= (MAX_error_targe + 1);
	}
	else
	{
		for (i = MAX_error_targe + 1; i < measureNum; i++)
		{
			Average_data += Filterdata[i];
		}
		Average_data /= (measureNum - MAX_error_targe - 1);
	}
	return Average_data;
}

float Median_filter_float(float data, int measureNum, float *Filterdata)
{
	unsigned int i = 0;
	unsigned int j = 0;
	float temp;
	unsigned int MAX_error_targe = 0;
	float MAX_error1;
	Filterdata[measureNum - 1] = data;
	for (i = 0; i < measureNum - 1; i++)
	{
		Filterdata[i] = Filterdata[i + 1];
	}
	for (i = 0; i < measureNum - 1; i++)
	{
		for (j = 0; j < measureNum - 1 - i; j++)
		{
			if (Filterdata[j] > Filterdata[j + 1])
			{
				temp = Filterdata[j];
				Filterdata[j] = Filterdata[j + 1];
				Filterdata[j + 1] = temp;
			}
		}
	}
	MAX_error1 = Filterdata[1] - Filterdata[0];
	for (i = 1; i < measureNum - 1; i++)
	{
		if (MAX_error1 < Filterdata[i + 1] - Filterdata[i])
		{
			MAX_error1 = Filterdata[i + 1] - Filterdata[i];
			MAX_error_targe = i;
		}
	}
	float Average_data = 0;
	if (MAX_error_targe + 1 > (measureNum + 1) / 2)
	{
		for (i = 0; i <= MAX_error_targe; i++)
		{
			Average_data += Filterdata[i];
		}
		Average_data /= (MAX_error_targe + 1);
	}
	else
	{
		for (i = MAX_error_targe + 1; i < measureNum; i++)
		{
			Average_data += Filterdata[i];
		}
		Average_data /= (measureNum - MAX_error_targe - 1);
	}
	return Average_data;
}
