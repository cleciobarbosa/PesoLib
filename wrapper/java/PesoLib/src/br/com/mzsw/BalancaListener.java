package br.com.mzsw;

/**
 * Interface que recebe os eventos de recebimento de peso e conex�o com uma balan�a
 * 
 * @author Mazin
 *
 */
public interface BalancaListener {
	/**
	 * Conectou-se a uma balan�a
	 * 
	 * @param sender instancia da balan�a
	 */
	public void onConectado(Object sender);
	
	/**
	 * Recebeu um peso da balan�a
	 * 
	 * @param sender instancia da balan�a
	 * @param gramas peso em gramas do item que est� sobre a balan�a
	 */
	public void onPesoRecebido(Object sender, int gramas);
	
	/**
	 * Desconectou-se de uma balan�a
	 * 
	 * @param sender instancia da balan�a
	 */
	public void onDesconectado(Object sender);
}
