/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   couleur.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbesse <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/30 14:02:00 by cbesse            #+#    #+#             */
/*   Updated: 2018/04/12 14:31:21 by cbesse           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"
#define AMBIANT 0

t_vecteur	r_background(t_ray *ray)
{
	t_vecteur	v2;
	t_vecteur	v1;
	t_vecteur	vr;
	double		t;

	v1 = v_set(1.0, 1.0, 1.0);
	v2 = v_set(0.5, 0.7, 1.0);
	t = 0.5 * (ray->dir.y + 1.0);
	vr = v_add(v_mult(v1, (1.0 - t)), v_mult(v2, t));
	return (vr);
}

t_vecteur	diffu_spec(t_vecteur light, t_record *r)
{
	t_vecteur	temp;
	t_vecteur	refrac;
	t_vecteur	diffu;
	double		shade;
	double		phong;

	temp = v_normalize(v_less(light, r->p));
	refrac = v_less(v_mult(r->normal, 2 * v_dot(r->normal, temp)), temp);
	phong = v_dot(refrac, temp);
	shade = v_dot(r->normal, temp);
	shade = shade < 0 ? 0 : shade;
	phong = phong < 0 ? 0 : phong;
	phong = pow(phong, 16);
	diffu.x = r->color.x * (AMBIANT + (1 - AMBIANT) * shade) + phong;
	diffu.y = r->color.y * (AMBIANT + (1 - AMBIANT) * shade) + phong;
	diffu.z = r->color.z * (AMBIANT + (1 - AMBIANT) * shade) + phong;
	return (diffu);
}

t_vecteur	c_shadow(t_vecteur *light, t_record *r, t_vecteur vr, int n_light)
{
	vr = v_add(diffu_spec(light[n_light], &r[0]), vr);
	vr.x = vr.x > 1 ? 1 : vr.x;
	vr.y = vr.y > 1 ? 1 : vr.y;
	vr.z = vr.z > 1 ? 1 : vr.z;
	return (vr);
}

t_vecteur	r_color(t_ray *ray, t_formlist *list, t_vecteur *light, int n_light)
{
	t_vecteur	vr;
	t_record	*r;
	double		*min_max;
	t_ray		sray;

	r = (t_record*)ft_memalloc(sizeof(t_record) * 2);
	min_max = (double *)ft_memalloc(2 * sizeof(double));
	set_min_max(0.0, DBL_MAX, min_max);
	vr = v_set(0, 0, 0);
	if (hit_qqch(list, ray, min_max, &r[0]))
	{
		while (n_light-- > 0)
		{
			set_min_max(0.01, 1, min_max);
			sray.ori = v_mult(r[0].p, 1.00001);
			sray.dir = v_less(light[n_light], r[0].p);
			if (!(hit_qqch(list, &sray, min_max, &r[1])))
				vr = c_shadow(light, &r[0], vr, n_light);
		}
		return (libe((void **)&r, (void **)&min_max, vr));
	}
	return (libe((void **)&r, (void **)&min_max, r_background(ray)));
}
