using UnityEngine;

public class Hammer : MonoBehaviour
{
    public bool swing = true;
    Rigidbody rb;

    private void Awake()
    {
        rb = GetComponent<Rigidbody>();
    }

    // Update is called once per frame
    void Update()
    {
        //change behavior of swing here
        //if (rb.velocity.y < -1)
        //{
        //    swing = true;
        //}
        //else
        //{
        //    swing = false;
        //}
    }
    private void OnTriggerEnter(Collider other)
    {
        GameObject parent = other.transform.parent.gameObject;
        
        if (swing == true)
        {
            //swing = false;
            if (parent.CompareTag("Ingot"))
            {
                GameObject rod = Instantiate(Resources.Load<GameObject>("Project/Prefabs/Rod"), parent.transform);
                rod.transform.parent = null;
                Destroy(parent);
            }
            else if (parent.CompareTag("Rod"))
            {
                GameObject plate = Instantiate(Resources.Load<GameObject>("Project/Prefabs/Plate"), parent.transform);
                plate.transform.parent = null;
                Destroy(parent);
            }
        }
        else
        {
            //null
        }
    }

}
