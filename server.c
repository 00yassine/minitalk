/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykabili- <ykabili-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 03:31:14 by ykabili-          #+#    #+#             */
/*   Updated: 2025/03/17 04:22:04 by ykabili-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minitalk.h"

int		g_lastclient;

void	ft_check_client(int *bit, int *byte, siginfo_t *info)
{
	if (g_lastclient != info->si_pid)
	{
		*bit = 0;
		*byte = 0;
		g_lastclient = info->si_pid;
	}
}

void	ft_handler(int signum, siginfo_t *info, void *context)
{
	static int	byte = 0;
	static int	bit = 0;

	(void)context;
	ft_check_client(&bit, &byte, info);
	if (signum == SIGUSR1)
		byte |= (1 << (7 - bit));
	bit++;
	if (bit == 8)
	{
		if (byte == '\0')
		{
			ft_putchar_fd('\n', 1);
			kill(info->si_pid, SIGUSR2);
		}
		else
			ft_putchar_fd(byte, 1);
		bit = 0;
		byte = 0;
	}
	kill(info->si_pid, SIGUSR1);
}

void	sig_setup(int sig, void (*handler1)(int, siginfo_t *, void *))
{
	struct sigaction	sigo;

	sigo.sa_sigaction = handler1;
	sigo.sa_flags = SA_SIGINFO;
	sigemptyset(&sigo.sa_mask);
	sigaction(sig, &sigo, NULL);
}

int	main(int ac, char **av)
{
	(void)av;
	if (ac != 1)
		return (ft_putstr_fd("Invalid arguments\nTry: ./server", 1), 1);
	ft_putstr_fd("The PID of this server is: ", 1);
	ft_putnbr_fd(getpid(), 1);
	ft_putchar_fd('\n', 1);
	sig_setup(SIGUSR1, ft_handler);
	sig_setup(SIGUSR2, ft_handler);
	while (1)
		pause();
}
