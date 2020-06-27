package rmi;

import java.rmi.RemoteException;
import java.rmi.server.UnicastRemoteObject;

public class RemoteMath extends UnicastRemoteObject implements IRemoteMath{
	/**
	 * 
	 */
	private static final long serialVersionUID = 3547707068591545355L;
	private static int numberOfComputations;
	
	protected RemoteMath() throws RemoteException {
		super();
		// TODO Auto-generated constructor stub
		numberOfComputations = 0;
	}
	
	@Override
	public double add(double a, double b) {
		numberOfComputations ++;
		System.out.println("Number of computations performed so far: " + numberOfComputations);
		return (a + b);
	}
	
	@Override
	public double substract(double a, double b) {
		numberOfComputations ++;
		System.out.println("Number of computations performed so far: " + numberOfComputations);
		return (a - b);
	}
}
