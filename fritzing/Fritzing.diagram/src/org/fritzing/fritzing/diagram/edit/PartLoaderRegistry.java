/*
 * (c) Fachhochschule Potsdam
 */
package org.fritzing.fritzing.diagram.edit;

import java.util.HashMap;
import java.util.Map;

/**
 * Manages PartLoader resources.
 * 
 */


public class PartLoaderRegistry {

	/**
	 * Singleton instance for the partLoader registry.
	 */
	private static PartLoaderRegistry singletonInstance = new PartLoaderRegistry();

	/**
	 * Return singleton instance of the partLoader registry.
	 * 
	 * @return the PartLoader registry
	 */
	public static PartLoaderRegistry getInstance() {
		return singletonInstance;
	}

	// HashMap that holds onto partLoader instances
	private final static Map<String, PartLoader> partLoaderRegistry = new HashMap<String, PartLoader>();

	/**
	 * Private constructor.
	 */
	private PartLoaderRegistry() {
		super();
	}

	/**
	 * Returns the PartLoader based on the path. If the PartLoader does not exist in the
	 * cache, creates a new one and caches.
	 * 
	 * @param path -
	 *            a string, either genus.species or the relative path (e.g. libraries/core/arduino/partDescription.xml)
	 * @return PartLoader
	 */
	public PartLoader get(String path) {
		PartLoader value = partLoaderRegistry.get(path);
		if (value != null) {
			return value;
		}
				
		value = new PartLoader();
		boolean result = value.loadXMLFromLibrary(path);
		if (result == false) return null;
		
		partLoaderRegistry.put(path, value);
		if (value.genus != null && value.species != null) {
			partLoaderRegistry.put(value.genus + value.species, value);
		}
		return value;
	}


}

