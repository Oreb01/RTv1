/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbesse <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/30 14:03:05 by cbesse            #+#    #+#             */
/*   Updated: 2018/03/30 14:03:07 by cbesse           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"

int main(int ac, char **av)
{

  ac = 0;
  //ft_parseur(av);
  t_mlx mlx;
	t_vecteur couleur;
  double i;
	int n = 0;
	int ir;
	int ig;
	int ib;
	double u;
	double v;
  t_scene *scene;
    scene = (t_scene *)ft_memalloc(sizeof(t_scene));
	t_vecteur temp[3];
	t_ray ray;
	t_camera cam;
	double j = W_WIDTH - 1;

	mlx_set(&mlx);
  ft_parseur(av, scene);
  cam = s_cam(scene->camorigin, scene->camdir, v_set(0,1,0), 90);
  while (j >= 0)
  {
		i = 0;
      while (i < W_LENGHT)
			{
				u = (double)i / (double)W_LENGHT;
				v = (double)j / (double)W_WIDTH;
				ray.A = cam.origin;
				temp[0] = v_mult(cam.horizontal, u);
				temp[1] = v_mult(cam.vertical, v);
				temp[2] = v_add(cam.lowleftcor, temp[0]);
				ray.B = v_less(v_add(temp[1], temp[2]), cam.origin);
				couleur = r_color(&ray, scene->list, scene->light, scene->n_light);
				ir = (int)(255.99 * couleur.x);
				ig = (int)(255.99 * couleur.y);
				ib = (int)(255.99 * couleur.z);
				mlx.img.data[n] = ir * 256 * 256 + ig * 256 + ib;
				//mlx.img.data[n] = color;
				n++;
				i++;
			}
			j--;
  }
  mlx_put_image_to_window(mlx.mlx, mlx.win, mlx.img.img_ptr, 0, 0);
  mlx_key_hook(mlx.win, my_key_funct, &mlx);
  mlx_loop(mlx.mlx);

}
