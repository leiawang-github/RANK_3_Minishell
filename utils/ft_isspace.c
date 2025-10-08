/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isspace.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rtiberio <rtiberio@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/13 09:31:10 by codespace         #+#    #+#             */
/*   Updated: 2025/08/16 18:52:22 by rtiberio         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_isspace(int c)
{
	unsigned char	uc;

	uc = (unsigned char)c;
	if (uc == 32 || (uc >= 9 && uc <= 13))
		return (1);
	return (0);
}
