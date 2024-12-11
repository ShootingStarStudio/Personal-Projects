using UnityEngine;
using UnityEngine.XR.Interaction.Toolkit;

public class GrabbedObject : MonoBehaviour
{
    private XRGrabInteractable grabInteractable;
    public bool Released = false;

    private void Awake()
    {
        grabInteractable = GetComponent<XRGrabInteractable>();
        grabInteractable.selectEntered.AddListener(OnGrab);
        grabInteractable.selectExited.AddListener(OnRelease);
    }

    private void OnGrab(SelectEnterEventArgs args)
    {
        GameObject grabbedObject = args.interactableObject.transform.gameObject;
    }

    private void OnRelease(SelectExitEventArgs interactor)
    {
        // Handle release logic if needed
        Released = true;
    }
}