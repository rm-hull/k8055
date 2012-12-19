/*  
 * Copyright (C) 2008 Martin Pischky (mailto:martin@pischky.de)  
 *
 * This file (JK8055Exception.java) is part of libk8055/jk8055.
 * jk8055 - a java wrapper for libk8055
 *
 * libk8055/jk8055 is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA.
 *
 * $Id: JK8055Exception.java,v 1.3 2008/07/14 07:47:55 pischky Exp $
 * 
 */

package net.sf.libk8055.jk8055;

/**
 * Exceptions thrown by class JK8055.
 * 
 * @author Martin Pischky
 */
public class JK8055Exception extends Exception {

	public static final String CVS_ID = "$Id: JK8055Exception.java,v 1.3 2008/07/14 07:47:55 pischky Exp $";

	/**
	 * 
	 */
	private static final long serialVersionUID = 3821577565947420161L;

	/**
	 * Constructor.
	 */
	public JK8055Exception() {
		super();
	}

	/**
	 * Constructor.
	 * @param message
	 */
	public JK8055Exception(String message) {
		super(message);
	}

	/**
	 * Constructor.
	 * @param cause
	 */
	public JK8055Exception(Throwable cause) {
		super(cause);
	}

	/**
	 * Constructor.
	 * @param message
	 * @param cause
	 */
	public JK8055Exception(String message, Throwable cause) {
		super(message, cause);
	}

}
