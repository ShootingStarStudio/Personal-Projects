using UnityEngine;

public class Rod : MonoBehaviour
{
    private void OnCollisionEnter(Collision collision)
    {
        Debug.Log("collided");
        GameObject parent = collision.gameObject;

        if (parent.CompareTag("Plate"))
        {
            GameObject axe = Instantiate(Resources.Load<GameObject>("Project/Prefabs/Axe"), parent.transform);
            axe.transform.parent = null;
            Destroy(gameObject);
            Destroy(parent);
        }
    }
}
