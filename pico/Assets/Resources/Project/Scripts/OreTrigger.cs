using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class OreTrigger : MonoBehaviour
{
    public List<GameObject> orestorage;
    // Update is called once per frame

    private void OnTriggerEnter(Collider other)
    {
        if (other.transform.parent.CompareTag("Ore"))
        {
            orestorage.Add(other.transform.parent.gameObject);
        }
    }

    private void OnTriggerExit(Collider other)
    {
        if (other.transform.parent.CompareTag("Ore"))
        {
            orestorage.Remove(other.transform.parent.gameObject);
        }
    }

    public void smelt()
    {
        GameObject index = orestorage[Random.Range(0, orestorage.Count)];
        orestorage.Remove(index);
        Destroy(index);
    }
}
