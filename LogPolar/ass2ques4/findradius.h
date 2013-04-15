#ifndef FINDRAIUS_H
#define FINDRAIUS_H
int find_radius(int x, int y, int height, int width)
{
	int radius;
	int temp_y,temp_x;

	if((int)(height/2 + 0.5) != x)
	{
		if((height - x) < x)
		{
			temp_x = height - x;
		}
		else
		{
			temp_x = x;
		}
	}
	else
	{
		temp_x = x;
	}

	if ((int)(width/2 + 0.5) != y)
	{
		if ((width - y) < y)
		{
			temp_y = width - y;
		}
		else
		{
			temp_y = y;
		}
	}
	else
	{
		temp_y = y;
	}


	if (temp_y < temp_x)
	{
		radius = temp_y;
	}
	else
	{
		radius = temp_x;
	}

	return radius;
}

#endif