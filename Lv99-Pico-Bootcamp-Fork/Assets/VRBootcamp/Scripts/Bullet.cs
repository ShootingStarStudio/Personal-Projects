using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Bullet : MonoBehaviour
{
    public enum BulletType // Corresponding gun
    {
        Pistol,
        Rifle
    }

    float FlightTime; // Time bullet Max live time
    BulletType bulletType;

    private void Update()
    {
        //FlightTime += Time.deltaTime;
        //if (FlightTime > 2)
        //    gameObject.SetActive(false);
    }

    private void OnCollisionEnter(Collision collision)
    {
        // Check if colliding with pistol
        if (collision.gameObject.tag != "Pistol" && collision.gameObject.tag != "Rifle")
        {
            // If not pistol during collision, destroy bullet game object
            Destroy(gameObject);
        }
        if (collision.gameObject.tag == "Target")
        {
            collision.gameObject.SetActive(false);
            // Add score
            collision.gameObject.transform.parent.GetComponent<ShootingTargetField>().AddScore();
        }
    }
}
