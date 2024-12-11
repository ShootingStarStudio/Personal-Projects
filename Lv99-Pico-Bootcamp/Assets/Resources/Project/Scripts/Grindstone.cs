using UnityEngine;

public class Grindstone : MonoBehaviour
{
    private void OnTriggerEnter(Collider other)
    {
        GameObject parent = other.transform.parent.gameObject;

            if (parent.CompareTag("Rod"))
            {
                GameObject sword = Instantiate(Resources.Load<GameObject>("Project/Prefabs/Sword"), parent.transform);
                sword.transform.parent = null;
                Destroy(parent);
            }
    }
}
