/*
 * (c) Fachhochschule Potsdam
 */
package org.fritzing.fritzing.diagram.edit.commands;

import org.eclipse.emf.ecore.EClass;
import org.eclipse.emf.ecore.EObject;
import org.eclipse.gmf.runtime.emf.type.core.commands.CreateElementCommand;
import org.eclipse.gmf.runtime.emf.type.core.requests.CreateElementRequest;
import org.eclipse.gmf.runtime.notation.View;
import org.fritzing.fritzing.FritzingPackage;
import org.fritzing.fritzing.Potentiometer;
import org.fritzing.fritzing.diagram.edit.PartLoader;
import org.fritzing.fritzing.diagram.providers.FritzingElementTypes;

/**
 * @generated
 */
public class PotentiometerCreateCommand extends CreateElementCommand {

	/**
	 * @generated
	 */
	public PotentiometerCreateCommand(CreateElementRequest req) {
		super(req);
	}

	/**
	 * @generated
	 */
	protected EObject getElementToEdit() {
		EObject container = ((CreateElementRequest) getRequest())
				.getContainer();
		if (container instanceof View) {
			container = ((View) container).getElement();
		}
		return container;
	}

	/**
	 * @generated
	 */
	protected EClass getEClassToEdit() {
		return FritzingPackage.eINSTANCE.getComposite();
	}

	/**
	 * @generated NOT
	 */
	protected EObject doDefaultElementCreation() {
		Potentiometer newElement = (Potentiometer) super
				.doDefaultElementCreation();
		if (newElement != null) {
			FritzingElementTypes.Initializers.Potentiometer_2005
					.init(newElement);
		}
		// use "our" initializers instead		
		PartLoader partLoader = new PartLoader();
		partLoader.createTerminals("libraries/core/potentiometer/partdescription.xml", newElement);		
		return newElement;
	}
}
