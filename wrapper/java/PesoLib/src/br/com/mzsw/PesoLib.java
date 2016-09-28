package br.com.mzsw;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;

/**
 * Biblioteca para conex�o com balan�as.
 * obt�m o peso de balan�as, realiza conex�o autom�tica
 * 
 * @author Mazin
 *
 */
public class PesoLib implements Runnable {
	private List<BalancaListener> listeners;
	private PesoLibWrapper driver;
	private long instance;
	private boolean canceled;
	private int ultimoPeso;
	
	/**
	 * Cria uma conex�o com uma balan�a
	 * 
	 * @param configuracao
	 */
	public PesoLib() {
		listeners = new ArrayList<>();
		driver = new PesoLibWrapper();
		instance = driver.criar("");
		if(instance == 0)
			throw new RuntimeException("N�o foi poss�vel criar uma inst�ncia da biblioteca");
		Thread thread = new Thread(this);
		thread.start();
	}
	
	/**
	 * Cria uma conex�o com uma balan�a especificando configura��o de porta e tempo
	 * 
	 * @param configuracao
	 */
	public PesoLib(String configuracao) {
		listeners = new ArrayList<>();
		driver = new PesoLibWrapper();
		instance = driver.criar(configuracao);
		if(instance == 0)
			throw new RuntimeException("N�o foi poss�vel criar uma inst�ncia da biblioteca");
		Thread thread = new Thread(this);
		thread.start();
	}
	
	/**
	 * Adiciona uma interface que receber� evento de conex�o e recebimento de peso
	 * 
	 * @param l interface que receber� os eventos
	 */
	public void addEventListener(BalancaListener l) {
		listeners.add(l);
	}
	
	/**
	 * Fecha a conex�o com a balan�a, ap�s a chamada desse m�todo, a instancia da classe n�o poder� mais ser usada
	 * 
	 */
	public void fecha() {
		if(canceled)
			return;
		canceled = true;
		driver.cancela(instance);
		driver.libera(instance);
		instance = 0;
	}
	
	private void needActive() {
		if(canceled)
			throw new RuntimeException("A instancia da biblioteca j� foi liberada");	
	}
	
	/**
	 * Informa para a balan�a o pre�o do item que est� sendo pesado
	 * 
	 * @param preco pre�o do item
	 */
	public void setPreco(float preco) {
		needActive();
		if(!driver.solicitaPeso(instance, preco))
			throw new RuntimeException("N�o foi poss�vel ajustar o pre�o do item da bala�a");
	}
	
	/**
	 * Solicita o peso do item sobre a balan�a
	 * 
	 */
	public void askPeso() {
		setPreco(0.0f);
	}

	/**
	 * Informa se est� conectado � uma balan�a
	 * 
	 * @return true se est� conectado, falso caso contr�rio
	 */
	public boolean isConectado() {
		if(canceled)
			return true;
		return driver.isConectado(instance);
	}
	
	/**
	 * Ajusta configura��es de conex�o com a balan�a e tempo de espera
	 * 
	 * @param configuracao configura��es com instru��es separadas por ;
	 * exemplo: port:COM3;baund:9600, ajusta a porta e a velocidade de conex�o
	 */
	public void setConfiguracao(String configuracao) {
		needActive();
		driver.setConfiguracao(instance, configuracao);
	}
	
	/**
	 * Obt�m a configura��o da conex�o atual
	 * 
	 * @return configura��es com instru��es separadas por ;
	 * exemplo: port:COM3;baund:9600, o primero � a porta e segundo � a velocidade de conex�o 
	 */
	public String getConfiguracao() {
		needActive();
		return driver.getConfiguracao(instance);
	}
	

	/**
	 * Obt�m o �ltimo peso enviado pela balan�a
	 * 
	 * @return peso em gramas
	 */
	public int getUltimoPeso() {
		return ultimoPeso;
	}

	/**
	 * Obt�m todas as marcas de balan�as suportadas pela biblioteca
	 * 
	 * @return lista com as marcas suportadas
	 */
	public List<String> getMarcas() {
		needActive();
		String marcas = driver.getMarcas(instance);
		return Arrays.asList(marcas.split("\r\n"));
	}
	
	/**
	 * Obt�m todas os modelos suportados pela balan�a da marca informada
	 * 
	 * @param marca marca da balan�a
	 * @return lista com todos os modelos suportados
	 */
	public List<String> getModelos(String marca) {
		needActive();
		String modelos = driver.getModelos(instance, marca);
		return Arrays.asList(modelos.split("\r\n"));
	}
	
	/**
	 * Obt�m a vers�o da biblioteca
	 * 
	 * @return vers�o no formato 0.0.0.0
	 */
	public String getVersao() {
		return driver.getVersao(instance);
	}
	
	@Override
	public void run() {
		int event;
		
		do {
			event = driver.aguardaEvento(instance);
			switch (event) {
			case PesoLibWrapper.EVENTO_CANCELADO:
				break;
			case PesoLibWrapper.EVENTO_CONECTADO:
				postEventConnect();
				break;
			case PesoLibWrapper.EVENTO_DESCONECTADO:
				postEventDisconnect();
				break;
			default:
				postEventWeightReceived(driver.getUltimoPeso(instance));
			}
		} while(!canceled && event != PesoLibWrapper.EVENTO_CANCELADO);
	}

	private void postEventConnect() {
		for (BalancaListener elem : listeners) {
			elem.onConectado(this);
		}
	}

	private void postEventDisconnect() {
		for (BalancaListener elem : listeners) {
			elem.onDesconectado(this);
		}
	}

	private void postEventWeightReceived(int ultimoPeso) {
		this.ultimoPeso = ultimoPeso;
		for (BalancaListener elem : listeners) {
			elem.onPesoRecebido(this, ultimoPeso);
		}
	}

}
