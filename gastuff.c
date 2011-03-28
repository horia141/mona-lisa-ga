typedef struct _individual  individual;
typedef struct _population  population;

struct _individual
{
  int         gene_cnt;
  rectangle*  genes;
};

struct _population
{
  int          indi_cnt;
  individual*  indis;
};

struct _image
{
  int     rows;
  int     cols;
  color*  data;
};

image*
individual_to_image(
  image* img,
  const individual* indi)
{
  // assert(image_is_valid(img));
  // assert(individual_is_valid(indi));

  color  color_sum;
  int    intersect_cnt;
  int    gene_x_scaled;
  int    gene_y_scaled;
  int    gene_r_scaled;
  int    gene_c_scaled;
  int    i;
  int    j;
  int    k;
  
  for (i = 0; i < image_rows(img); i++) {
    for (j = 0; j < image_cols(img); j++) {
      color_sum.r = 0;
      color_sum.g = 0;
      color_sum.b = 0;
      color_sum.a = 0;
      intersect_cnt = 0;

      for (k = 0; k < indi->gene_cnt; k++) {
	gene_x_scaled = (int)(indi->gene[i].x * image_rows(img));
	gene_y_scaled = (int)(indi->gene[i].y * image_rows(img));
	gene_r_scaled = (int)(indi->gene[i].r * image_rows(img));
	gene_c_scaled = (int)(indi->gene[i].c * image_rows(img));

	if (i >= x && i <= x + r &&
	    j >= y && j <= y + c) {
	  color_sum.r += indi->gene[i].color.r;
	  color_sum.g += indi->gene[i].color.g;
	  color_sum.b += indi->gene[i].color.b;
	  color_sum.a += indi->gene[i].color.a;
	  intersect_cnt += 1;
	}
      }

      color_sum.r /= (float)intersect_cnt;
      color_sum.g /= (float)intersect_cnt;
      color_sum.b /= (float)intersect_cnt;
      color_sum.a /= (float)intersect_cnt;

      image_set(img,i,j,color_sum);
    }
  }
}
