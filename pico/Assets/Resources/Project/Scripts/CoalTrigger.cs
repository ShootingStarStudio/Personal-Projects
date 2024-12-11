using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class CoalTrigger : MonoBehaviour
{
    public List<GameObject> coalstorage;
    // Update is called once per frame

    private void OnTriggerEnter(Collider other)
    {
        if (other.transform.parent.CompareTag("Coal"))
        {
            coalstorage.Add(other.transform.parent.gameObject);
        } 
    }

    private void OnTriggerExit(Collider other)
    {
        if (other.transform.parent.CompareTag("Coal"))
        {
            coalstorage.Remove(other.transform.parent.gameObject);
        }
    }

    public void smelt()
    {
        GameObject index = coalstorage[Random.Range(0, coalstorage.Count)];
        coalstorage.Remove(index);
        Destroy(index);
    }
}
