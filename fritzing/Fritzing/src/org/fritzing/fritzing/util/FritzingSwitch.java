/**
 * (c) Fachhochschule Potsdam
 *
 * $Id$
 */
package org.fritzing.fritzing.util;

import java.util.List;

import org.eclipse.emf.ecore.EClass;
import org.eclipse.emf.ecore.EObject;

import org.fritzing.fritzing.*;

/**
 * <!-- begin-user-doc -->
 * The <b>Switch</b> for the model's inheritance hierarchy.
 * It supports the call {@link #doSwitch(EObject) doSwitch(object)}
 * to invoke the <code>caseXXX</code> method for each class of the model,
 * starting with the actual class of the object
 * and proceeding up the inheritance hierarchy
 * until a non-null result is returned,
 * which is the result of the switch.
 * <!-- end-user-doc -->
 * @see org.fritzing.fritzing.FritzingPackage
 * @generated
 */
public class FritzingSwitch<T> {
	/**
	 * The cached model package
	 * <!-- begin-user-doc -->
	 * <!-- end-user-doc -->
	 * @generated
	 */
	protected static FritzingPackage modelPackage;

	/**
	 * Creates an instance of the switch.
	 * <!-- begin-user-doc -->
	 * <!-- end-user-doc -->
	 * @generated
	 */
	public FritzingSwitch() {
		if (modelPackage == null) {
			modelPackage = FritzingPackage.eINSTANCE;
		}
	}

	/**
	 * Calls <code>caseXXX</code> for each class of the model until one returns a non null result; it yields that result.
	 * <!-- begin-user-doc -->
	 * <!-- end-user-doc -->
	 * @return the first non-null result returned by a <code>caseXXX</code> call.
	 * @generated
	 */
	public T doSwitch(EObject theEObject) {
		return doSwitch(theEObject.eClass(), theEObject);
	}

	/**
	 * Calls <code>caseXXX</code> for each class of the model until one returns a non null result; it yields that result.
	 * <!-- begin-user-doc -->
	 * <!-- end-user-doc -->
	 * @return the first non-null result returned by a <code>caseXXX</code> call.
	 * @generated
	 */
	protected T doSwitch(EClass theEClass, EObject theEObject) {
		if (theEClass.eContainer() == modelPackage) {
			return doSwitch(theEClass.getClassifierID(), theEObject);
		}
		else {
			List<EClass> eSuperTypes = theEClass.getESuperTypes();
			return
				eSuperTypes.isEmpty() ?
					defaultCase(theEObject) :
					doSwitch(eSuperTypes.get(0), theEObject);
		}
	}

	/**
	 * Calls <code>caseXXX</code> for each class of the model until one returns a non null result; it yields that result.
	 * <!-- begin-user-doc -->
	 * <!-- end-user-doc -->
	 * @return the first non-null result returned by a <code>caseXXX</code> call.
	 * @generated
	 */
	protected T doSwitch(int classifierID, EObject theEObject) {
		switch (classifierID) {
			case FritzingPackage.DOCUMENT_ROOT: {
				DocumentRoot documentRoot = (DocumentRoot)theEObject;
				T result = caseDocumentRoot(documentRoot);
				if (result == null) result = defaultCase(theEObject);
				return result;
			}
			case FritzingPackage.TERMINAL: {
				Terminal terminal = (Terminal)theEObject;
				T result = caseTerminal(terminal);
				if (result == null) result = caseElement(terminal);
				if (result == null) result = caseITrackConnection(terminal);
				if (result == null) result = caseILegConnection(terminal);
				if (result == null) result = caseIWireConnection(terminal);
				if (result == null) result = caseIElement(terminal);
				if (result == null) result = defaultCase(theEObject);
				return result;
			}
			case FritzingPackage.ELEMENT: {
				Element element = (Element)theEObject;
				T result = caseElement(element);
				if (result == null) result = caseIElement(element);
				if (result == null) result = defaultCase(theEObject);
				return result;
			}
			case FritzingPackage.CONNECTION: {
				Connection connection = (Connection)theEObject;
				T result = caseConnection(connection);
				if (result == null) result = caseElement(connection);
				if (result == null) result = caseIElement(connection);
				if (result == null) result = defaultCase(theEObject);
				return result;
			}
			case FritzingPackage.PART: {
				Part part = (Part)theEObject;
				T result = casePart(part);
				if (result == null) result = caseElement(part);
				if (result == null) result = caseIElement(part);
				if (result == null) result = defaultCase(theEObject);
				return result;
			}
			case FritzingPackage.GENERIC_PART: {
				GenericPart genericPart = (GenericPart)theEObject;
				T result = caseGenericPart(genericPart);
				if (result == null) result = casePart(genericPart);
				if (result == null) result = caseElement(genericPart);
				if (result == null) result = caseIElement(genericPart);
				if (result == null) result = defaultCase(theEObject);
				return result;
			}
			case FritzingPackage.RESISTOR: {
				Resistor resistor = (Resistor)theEObject;
				T result = caseResistor(resistor);
				if (result == null) result = casePart(resistor);
				if (result == null) result = caseElement(resistor);
				if (result == null) result = caseIElement(resistor);
				if (result == null) result = defaultCase(theEObject);
				return result;
			}
			case FritzingPackage.LED: {
				LED led = (LED)theEObject;
				T result = caseLED(led);
				if (result == null) result = casePart(led);
				if (result == null) result = caseElement(led);
				if (result == null) result = caseIElement(led);
				if (result == null) result = defaultCase(theEObject);
				return result;
			}
			case FritzingPackage.COMPOSITE: {
				Composite composite = (Composite)theEObject;
				T result = caseComposite(composite);
				if (result == null) result = casePart(composite);
				if (result == null) result = caseElement(composite);
				if (result == null) result = caseIElement(composite);
				if (result == null) result = defaultCase(theEObject);
				return result;
			}
			case FritzingPackage.SKETCH: {
				Sketch sketch = (Sketch)theEObject;
				T result = caseSketch(sketch);
				if (result == null) result = caseComposite(sketch);
				if (result == null) result = caseILegConnection(sketch);
				if (result == null) result = casePart(sketch);
				if (result == null) result = caseIElement(sketch);
				if (result == null) result = caseElement(sketch);
				if (result == null) result = defaultCase(theEObject);
				return result;
			}
			case FritzingPackage.MODULE: {
				Module module = (Module)theEObject;
				T result = caseModule(module);
				if (result == null) result = caseComposite(module);
				if (result == null) result = casePart(module);
				if (result == null) result = caseElement(module);
				if (result == null) result = caseIElement(module);
				if (result == null) result = defaultCase(theEObject);
				return result;
			}
			case FritzingPackage.BREADBOARD: {
				Breadboard breadboard = (Breadboard)theEObject;
				T result = caseBreadboard(breadboard);
				if (result == null) result = casePart(breadboard);
				if (result == null) result = caseElement(breadboard);
				if (result == null) result = caseIElement(breadboard);
				if (result == null) result = defaultCase(theEObject);
				return result;
			}
			case FritzingPackage.WIRE: {
				Wire wire = (Wire)theEObject;
				T result = caseWire(wire);
				if (result == null) result = caseConnection(wire);
				if (result == null) result = caseElement(wire);
				if (result == null) result = caseIElement(wire);
				if (result == null) result = defaultCase(theEObject);
				return result;
			}
			case FritzingPackage.TRACK: {
				Track track = (Track)theEObject;
				T result = caseTrack(track);
				if (result == null) result = caseConnection(track);
				if (result == null) result = caseITrackConnection(track);
				if (result == null) result = caseElement(track);
				if (result == null) result = caseIElement(track);
				if (result == null) result = defaultCase(theEObject);
				return result;
			}
			case FritzingPackage.ITRACK_CONNECTION: {
				ITrackConnection iTrackConnection = (ITrackConnection)theEObject;
				T result = caseITrackConnection(iTrackConnection);
				if (result == null) result = caseIElement(iTrackConnection);
				if (result == null) result = defaultCase(theEObject);
				return result;
			}
			case FritzingPackage.LEG: {
				Leg leg = (Leg)theEObject;
				T result = caseLeg(leg);
				if (result == null) result = caseConnection(leg);
				if (result == null) result = caseILegConnection(leg);
				if (result == null) result = caseIWireConnection(leg);
				if (result == null) result = caseElement(leg);
				if (result == null) result = caseIElement(leg);
				if (result == null) result = defaultCase(theEObject);
				return result;
			}
			case FritzingPackage.ILEG_CONNECTION: {
				ILegConnection iLegConnection = (ILegConnection)theEObject;
				T result = caseILegConnection(iLegConnection);
				if (result == null) result = caseIElement(iLegConnection);
				if (result == null) result = defaultCase(theEObject);
				return result;
			}
			case FritzingPackage.IWIRE_CONNECTION: {
				IWireConnection iWireConnection = (IWireConnection)theEObject;
				T result = caseIWireConnection(iWireConnection);
				if (result == null) result = caseIElement(iWireConnection);
				if (result == null) result = defaultCase(theEObject);
				return result;
			}
			case FritzingPackage.IELEMENT: {
				IElement iElement = (IElement)theEObject;
				T result = caseIElement(iElement);
				if (result == null) result = defaultCase(theEObject);
				return result;
			}
			default: return defaultCase(theEObject);
		}
	}

	/**
	 * Returns the result of interpreting the object as an instance of '<em>Document Root</em>'.
	 * <!-- begin-user-doc -->
	 * This implementation returns null;
	 * returning a non-null result will terminate the switch.
	 * <!-- end-user-doc -->
	 * @param object the target of the switch.
	 * @return the result of interpreting the object as an instance of '<em>Document Root</em>'.
	 * @see #doSwitch(org.eclipse.emf.ecore.EObject) doSwitch(EObject)
	 * @generated
	 */
	public T caseDocumentRoot(DocumentRoot object) {
		return null;
	}

	/**
	 * Returns the result of interpreting the object as an instance of '<em>Terminal</em>'.
	 * <!-- begin-user-doc -->
	 * This implementation returns null;
	 * returning a non-null result will terminate the switch.
	 * <!-- end-user-doc -->
	 * @param object the target of the switch.
	 * @return the result of interpreting the object as an instance of '<em>Terminal</em>'.
	 * @see #doSwitch(org.eclipse.emf.ecore.EObject) doSwitch(EObject)
	 * @generated
	 */
	public T caseTerminal(Terminal object) {
		return null;
	}

	/**
	 * Returns the result of interpreting the object as an instance of '<em>Element</em>'.
	 * <!-- begin-user-doc -->
	 * This implementation returns null;
	 * returning a non-null result will terminate the switch.
	 * <!-- end-user-doc -->
	 * @param object the target of the switch.
	 * @return the result of interpreting the object as an instance of '<em>Element</em>'.
	 * @see #doSwitch(org.eclipse.emf.ecore.EObject) doSwitch(EObject)
	 * @generated
	 */
	public T caseElement(Element object) {
		return null;
	}

	/**
	 * Returns the result of interpreting the object as an instance of '<em>Connection</em>'.
	 * <!-- begin-user-doc -->
	 * This implementation returns null;
	 * returning a non-null result will terminate the switch.
	 * <!-- end-user-doc -->
	 * @param object the target of the switch.
	 * @return the result of interpreting the object as an instance of '<em>Connection</em>'.
	 * @see #doSwitch(org.eclipse.emf.ecore.EObject) doSwitch(EObject)
	 * @generated
	 */
	public T caseConnection(Connection object) {
		return null;
	}

	/**
	 * Returns the result of interpreting the object as an instance of '<em>Wire</em>'.
	 * <!-- begin-user-doc -->
	 * This implementation returns null;
	 * returning a non-null result will terminate the switch.
	 * <!-- end-user-doc -->
	 * @param object the target of the switch.
	 * @return the result of interpreting the object as an instance of '<em>Wire</em>'.
	 * @see #doSwitch(org.eclipse.emf.ecore.EObject) doSwitch(EObject)
	 * @generated
	 */
	public T caseWire(Wire object) {
		return null;
	}

	/**
	 * Returns the result of interpreting the object as an instance of '<em>Track</em>'.
	 * <!-- begin-user-doc -->
	 * This implementation returns null;
	 * returning a non-null result will terminate the switch.
	 * <!-- end-user-doc -->
	 * @param object the target of the switch.
	 * @return the result of interpreting the object as an instance of '<em>Track</em>'.
	 * @see #doSwitch(org.eclipse.emf.ecore.EObject) doSwitch(EObject)
	 * @generated
	 */
	public T caseTrack(Track object) {
		return null;
	}

	/**
	 * Returns the result of interpreting the object as an instance of '<em>ITrack Connection</em>'.
	 * <!-- begin-user-doc -->
	 * This implementation returns null;
	 * returning a non-null result will terminate the switch.
	 * <!-- end-user-doc -->
	 * @param object the target of the switch.
	 * @return the result of interpreting the object as an instance of '<em>ITrack Connection</em>'.
	 * @see #doSwitch(org.eclipse.emf.ecore.EObject) doSwitch(EObject)
	 * @generated
	 */
	public T caseITrackConnection(ITrackConnection object) {
		return null;
	}

	/**
	 * Returns the result of interpreting the object as an instance of '<em>Leg</em>'.
	 * <!-- begin-user-doc -->
	 * This implementation returns null;
	 * returning a non-null result will terminate the switch.
	 * <!-- end-user-doc -->
	 * @param object the target of the switch.
	 * @return the result of interpreting the object as an instance of '<em>Leg</em>'.
	 * @see #doSwitch(org.eclipse.emf.ecore.EObject) doSwitch(EObject)
	 * @generated
	 */
	public T caseLeg(Leg object) {
		return null;
	}

	/**
	 * Returns the result of interpreting the object as an instance of '<em>ILeg Connection</em>'.
	 * <!-- begin-user-doc -->
	 * This implementation returns null;
	 * returning a non-null result will terminate the switch.
	 * <!-- end-user-doc -->
	 * @param object the target of the switch.
	 * @return the result of interpreting the object as an instance of '<em>ILeg Connection</em>'.
	 * @see #doSwitch(org.eclipse.emf.ecore.EObject) doSwitch(EObject)
	 * @generated
	 */
	public T caseILegConnection(ILegConnection object) {
		return null;
	}

	/**
	 * Returns the result of interpreting the object as an instance of '<em>IWire Connection</em>'.
	 * <!-- begin-user-doc -->
	 * This implementation returns null;
	 * returning a non-null result will terminate the switch.
	 * <!-- end-user-doc -->
	 * @param object the target of the switch.
	 * @return the result of interpreting the object as an instance of '<em>IWire Connection</em>'.
	 * @see #doSwitch(org.eclipse.emf.ecore.EObject) doSwitch(EObject)
	 * @generated
	 */
	public T caseIWireConnection(IWireConnection object) {
		return null;
	}

	/**
	 * Returns the result of interpreting the object as an instance of '<em>IElement</em>'.
	 * <!-- begin-user-doc -->
	 * This implementation returns null;
	 * returning a non-null result will terminate the switch.
	 * <!-- end-user-doc -->
	 * @param object the target of the switch.
	 * @return the result of interpreting the object as an instance of '<em>IElement</em>'.
	 * @see #doSwitch(org.eclipse.emf.ecore.EObject) doSwitch(EObject)
	 * @generated
	 */
	public T caseIElement(IElement object) {
		return null;
	}

	/**
	 * Returns the result of interpreting the object as an instance of '<em>Part</em>'.
	 * <!-- begin-user-doc -->
	 * This implementation returns null;
	 * returning a non-null result will terminate the switch.
	 * <!-- end-user-doc -->
	 * @param object the target of the switch.
	 * @return the result of interpreting the object as an instance of '<em>Part</em>'.
	 * @see #doSwitch(org.eclipse.emf.ecore.EObject) doSwitch(EObject)
	 * @generated
	 */
	public T casePart(Part object) {
		return null;
	}

	/**
	 * Returns the result of interpreting the object as an instance of '<em>Resistor</em>'.
	 * <!-- begin-user-doc -->
	 * This implementation returns null;
	 * returning a non-null result will terminate the switch.
	 * <!-- end-user-doc -->
	 * @param object the target of the switch.
	 * @return the result of interpreting the object as an instance of '<em>Resistor</em>'.
	 * @see #doSwitch(org.eclipse.emf.ecore.EObject) doSwitch(EObject)
	 * @generated
	 */
	public T caseResistor(Resistor object) {
		return null;
	}

	/**
	 * Returns the result of interpreting the object as an instance of '<em>LED</em>'.
	 * <!-- begin-user-doc -->
	 * This implementation returns null;
	 * returning a non-null result will terminate the switch.
	 * <!-- end-user-doc -->
	 * @param object the target of the switch.
	 * @return the result of interpreting the object as an instance of '<em>LED</em>'.
	 * @see #doSwitch(org.eclipse.emf.ecore.EObject) doSwitch(EObject)
	 * @generated
	 */
	public T caseLED(LED object) {
		return null;
	}

	/**
	 * Returns the result of interpreting the object as an instance of '<em>Composite</em>'.
	 * <!-- begin-user-doc -->
	 * This implementation returns null;
	 * returning a non-null result will terminate the switch.
	 * <!-- end-user-doc -->
	 * @param object the target of the switch.
	 * @return the result of interpreting the object as an instance of '<em>Composite</em>'.
	 * @see #doSwitch(org.eclipse.emf.ecore.EObject) doSwitch(EObject)
	 * @generated
	 */
	public T caseComposite(Composite object) {
		return null;
	}

	/**
	 * Returns the result of interpreting the object as an instance of '<em>Sketch</em>'.
	 * <!-- begin-user-doc -->
	 * This implementation returns null;
	 * returning a non-null result will terminate the switch.
	 * <!-- end-user-doc -->
	 * @param object the target of the switch.
	 * @return the result of interpreting the object as an instance of '<em>Sketch</em>'.
	 * @see #doSwitch(org.eclipse.emf.ecore.EObject) doSwitch(EObject)
	 * @generated
	 */
	public T caseSketch(Sketch object) {
		return null;
	}

	/**
	 * Returns the result of interpreting the object as an instance of '<em>Breadboard</em>'.
	 * <!-- begin-user-doc -->
	 * This implementation returns null;
	 * returning a non-null result will terminate the switch.
	 * <!-- end-user-doc -->
	 * @param object the target of the switch.
	 * @return the result of interpreting the object as an instance of '<em>Breadboard</em>'.
	 * @see #doSwitch(org.eclipse.emf.ecore.EObject) doSwitch(EObject)
	 * @generated
	 */
	public T caseBreadboard(Breadboard object) {
		return null;
	}

	/**
	 * Returns the result of interpreting the object as an instance of '<em>Module</em>'.
	 * <!-- begin-user-doc -->
	 * This implementation returns null;
	 * returning a non-null result will terminate the switch.
	 * <!-- end-user-doc -->
	 * @param object the target of the switch.
	 * @return the result of interpreting the object as an instance of '<em>Module</em>'.
	 * @see #doSwitch(org.eclipse.emf.ecore.EObject) doSwitch(EObject)
	 * @generated
	 */
	public T caseModule(Module object) {
		return null;
	}

	/**
	 * Returns the result of interpreting the object as an instance of '<em>Generic Part</em>'.
	 * <!-- begin-user-doc -->
	 * This implementation returns null;
	 * returning a non-null result will terminate the switch.
	 * <!-- end-user-doc -->
	 * @param object the target of the switch.
	 * @return the result of interpreting the object as an instance of '<em>Generic Part</em>'.
	 * @see #doSwitch(org.eclipse.emf.ecore.EObject) doSwitch(EObject)
	 * @generated
	 */
	public T caseGenericPart(GenericPart object) {
		return null;
	}

	/**
	 * Returns the result of interpreting the object as an instance of '<em>EObject</em>'.
	 * <!-- begin-user-doc -->
	 * This implementation returns null;
	 * returning a non-null result will terminate the switch, but this is the last case anyway.
	 * <!-- end-user-doc -->
	 * @param object the target of the switch.
	 * @return the result of interpreting the object as an instance of '<em>EObject</em>'.
	 * @see #doSwitch(org.eclipse.emf.ecore.EObject)
	 * @generated
	 */
	public T defaultCase(EObject object) {
		return null;
	}

} //FritzingSwitch
