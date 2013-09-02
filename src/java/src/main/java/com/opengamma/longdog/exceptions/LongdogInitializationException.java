package com.opengamma.longdog.exceptions;

public class LongdogInitializationException extends RuntimeException {

	private static final long serialVersionUID = 1L;

	public LongdogInitializationException() {
		super();
	}

	public LongdogInitializationException(String message) {
		super(message);
	}

	public LongdogInitializationException(Throwable cause) {
		super(cause);
	}

	public LongdogInitializationException(String message, Throwable cause) {
		super(message, cause);
	}

	public LongdogInitializationException(String message, Throwable cause,
			boolean enableSuppression, boolean writableStackTrace) {
		super(message, cause, enableSuppression, writableStackTrace);
	}

}
