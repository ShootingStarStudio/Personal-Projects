using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class ProjectileManager : MonoBehaviour
{
    List<Bullet> BulletList = new List<Bullet>(); // List of Existing Bullets

    [SerializeField] GameObject BulletPrefab; // Bullet Blueprint

    // To get an unused bullet
    public Bullet GetBullet()
    {
        for (int i = 0; i < BulletList.Count; i++)
        {
            GameObject bullet = BulletList[i].gameObject;
            if (!bullet.activeSelf) // checking for unused bullet
            {
                bullet.SetActive(true); // Use this bullet
                return BulletList[i];
            }
        }

        // If no unused bullet, Make a new bullet to use
        Bullet newBullet = Instantiate(BulletPrefab).GetComponent<Bullet>();

        BulletList.Add(newBullet);
        return newBullet;
    }
}
