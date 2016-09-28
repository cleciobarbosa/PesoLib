/*
    PesoLib - Biblioteca para obten��o do peso de itens de uma balan�a
    Copyright (C) 2010-2014 MZSW Creative Software

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

    MZSW Creative Software
    contato@mzsw.com.br
*/

/** @file PesoLib.h
 *  Main include header for the PesoLib library
 */

#ifndef _PESOLIB_H_
#define _PESOLIB_H_

#ifdef __cplusplus
extern "C" {
#endif

typedef enum PesoLibEvento
{
	/** A inst�ncia da conex�o com a balan�a est� sendo liberada */
	Evento_Cancelado = 0,
	/** Conex�o estabelecida com uma balan�a */
	Evento_Conectado,
	/** A balan�a foi desconectada */
	Evento_Desconectado,
	/** A balan�a enviou o peso para o computador */
	Evento_PesoRecebido
} PesoLibEvento;

typedef struct PesoLib PesoLib;

#ifdef BUILD_DLL
# define LIBEXPORT __declspec(dllexport)
#else
#  ifdef LIB_STATIC
#    define LIBEXPORT
#  else
#    define LIBEXPORT extern
#  endif
#endif
#define LIBCALL __stdcall

#include "private/Plataforma.h"

/**
 * Inicia a conex�o com uma nova balan�a
 * 
 * par�metros
 *   config: configura��o da porta
 * 
 * retorno
 *   um ponteiro para a conex�o com uma balan�a
 */
LIBEXPORT PesoLib * LIBCALL PesoLib_cria(const char* config);

/**
 * Verifica se foi estabelecido uma conex�o com uma balan�a
 * 
 * par�metros
 *   lib: ponteiro para a conex�o com uma balan�a
 * 
 * retorno
 *   um ponteiro para a conex�o com uma balan�a
 */
LIBEXPORT int LIBCALL PesoLib_isConectado(PesoLib * lib);

/**
 * Altera a configura��o de conex�o com a balan�a
 * 
 * par�metros
 *   lib: ponteiro para a conex�o com uma balan�a
 *   config: nova configura��o, contendo informa��es da conex�o
 */
LIBEXPORT void LIBCALL PesoLib_setConfiguracao(PesoLib * lib, const char * config);

/**
 * Obt�m a configura��o atual da conex�o com a balab�a
 * 
 * par�metros
 *   lib: ponteiro para a conex�o com uma balan�a
 * 
 * retorno
 *   uma lista de par�metros de conex�o separados por ;
 */
LIBEXPORT const char* LIBCALL PesoLib_getConfiguracao(PesoLib * lib);

/**
 * Obt�m todas as marcas suportadas pela biblioteca
 * 
 * par�metros
 *   lib: ponteiro para a conex�o com uma balan�a
 * 
 * retorno
 *   uma lista de marcas de balan�as separadas por quebra de linha \r\n
 */
LIBEXPORT const char* LIBCALL PesoLib_getMarcas(PesoLib * lib);

/**
 * Obt�m todos os modelos de balan�as suportadas de uma determinada marca
 * 
 * par�metros
 *   lib: ponteiro para a conex�o com uma balan�a
 *   marca: nome da marca a qual deseja-se obter os modelos
 * 
 * retorno
 *   uma lista de modelos de balan�as separadas por quebra de linha \r\n
 */
LIBEXPORT const char* LIBCALL PesoLib_getModelos(PesoLib * lib, const char* marca);

/**
 * Aguarda um evento de conex�o ou de recebimento de dados
 * 
 * par�metros
 *   lib: ponteiro para a conex�o com uma balan�a
 * 
 * retorno
 *   0 se a instancia da conex�o foi liberada,
 *   1 se uma conex�o foi estabelecida com uma balan�a,
 *   2 se a balan�a foi desconectada
 *   3 quando a balan�a envia o peso para o computador
 */
LIBEXPORT int LIBCALL PesoLib_aguardaEvento(PesoLib * lib);

/**
 * Obt�m o �ltimo peso recebido da balan�a
 * 
 * par�metros
 *   lib: ponteiro para a conex�o com uma balan�a
 * 
 * retorno
 *   �ltimo peso recebido pela balan�a
 */
LIBEXPORT int LIBCALL PesoLib_getUltimoPeso(PesoLib * lib);

/**
 * Aguarda o envio do peso pela balan�a
 * 
 * par�metros
 *   lib: ponteiro para a conex�o com uma balan�a
 *   gramas: vari�vel que recebe o peso em gramas, fornecido pela balan�a
 * 
 * retorno
 *   1 se recebeu o peso ou 0 se a conex�o foi finalizada
 */
LIBEXPORT int LIBCALL PesoLib_recebePeso(PesoLib * lib, int* gramas);

/**
 * Solicita o envio do peso pela balan�a e informa o pre�o do kilo 
 * para ser mostrado no visor da mesma
 * 
 * par�metros
 *   lib: ponteiro para a conex�o com uma balan�a
 *   preco: pre�o do item por quilo que est� sendo pesado
 * 
 * retorno
 *   1 se enviou a solicita��o com sucesso ou 0 se n�o existe balan�a conectada
 */
LIBEXPORT int LIBCALL PesoLib_solicitaPeso(PesoLib * lib, float preco);

/**
 * Cancela uma conex�o e libera as fun��es que aguardam evento
 * 
 * par�metros
 *   lib: ponteiro para a conex�o com uma balan�a
 */
LIBEXPORT void LIBCALL PesoLib_cancela(PesoLib * lib);

/**
 * Finaliza uma conex�o com uma balan�a
 * 
 * par�metros
 *   lib: ponteiro para a conex�o com uma balan�a
 */
LIBEXPORT void LIBCALL PesoLib_libera(PesoLib * lib);


/**
 * Obt�m a vers�o da biblioteca
 * 
 * par�metros
 *   lib: ponteiro para a conex�o com uma balan�a
 */
LIBEXPORT const char* LIBCALL PesoLib_getVersao(PesoLib * lib);

#ifdef __cplusplus
}
#endif

#endif /* _PESOLIB_H_ */