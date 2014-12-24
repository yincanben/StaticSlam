/*************************************************************************
	> File Name: pcl_segmentation.cpp
	> Author: yincanben
	> Mail: yincanben@163.com
	> Created Time: Sun 21 Dec 2014 10:37:39 AM CST
 ************************************************************************/
#include <pcl/ModelCoefficients.h>
#include <pcl/point_types.h>
#include <pcl/io/pcd_io.h>
#include <pcl/filters/passthrough.h>
#include <pcl/filters/extract_indices.h>
#include <pcl/filters/voxel_grid.h>
#include <pcl/features/normal_3d.h>
#include <pcl/kdtree/kdtree.h>
#include <pcl/sample_consensus/method_types.h>
#include <pcl/sample_consensus/model_types.h>
#include <pcl/segmentation/sac_segmentation.h>
#include <pcl/segmentation/extract_clusters.h>
using namespace std ;

int main (int argc, char** argv){
    // Read in the cloud data
    pcl::PCDReader reader ;
    pcl::PointCloud<pcl::PointXYZ>::Ptr cloud (new pcl::PointCloud<pcl::PointXYZ>), cloud_f (new pcl::PointCloud<pcl::PointXYZ>) ;
    reader.read ("../images/table_scene_lms400.pcd", *cloud) ;
    if (pcl::io::loadPCDFile<pcl::PointXYZ> ("../images/table_scene_lms400.pcd", *cloud) == -1) //* load the file
    {
        PCL_ERROR ("Couldn't read file test_pcd.pcd \n");
        return (-1);
    }
    cout << "PointCloud before filtering has: " << cloud->points.size () << " data points." << endl ; //*
    /*for (size_t i = 0; i < cloud->points.size (); ++i){
        if( cloud->points[i].z < 4.0 ){
            cout << " " << cloud->points[i].x << " " << cloud->points[i].y << " " << cloud->points[i].z << endl;
        }
    }*/
    //Create the filtering object
    /*
    pcl::PointCloud<pcl::PointXYZ>::Ptr cloud_pre_filter (new pcl::PointCloud<pcl::PointXYZ>) ;
    pcl::PassThrough<pcl::PointXYZ> pass;
    pass.setInputCloud (cloud);
    pass.setFilterFieldName ("z");
    pass.setFilterLimits (0.0, 4.0);
    //pass.setFilterFieldName("y");
    //pass.setFilterLimits()
    pass.setFilterLimitsNegative (true);
    pass.filter (*cloud_pre_filter);
    
    pcl::io::savePCDFileASCII("../images/pre_filtered.pcd", *cloud_pre_filter) ;
    cout << "PointCloud after filtering has: " << cloud_pre_filter->points.size () << " data points." << endl ; /
    */
    // Create the filtering object: downsample the dataset using a leaf size of 1cm
    pcl::VoxelGrid<pcl::PointXYZ> vg ;
    pcl::PointCloud<pcl::PointXYZ>::Ptr cloud_filtered (new pcl::PointCloud<pcl::PointXYZ>) ;
    vg.setInputCloud (cloud) ;
    vg.setLeafSize (0.01f, 0.01f, 0.01f) ;
    vg.filter (*cloud_filtered) ;
    cout << "PointCloud after filtering has: " << cloud_filtered->points.size ()  << " data points." << endl ; 
    //pcl::io::savePCDFileASCII ("../images/cloud_filtered_pcd.pcd", *cloud_filtered) ;

    // Create the segmentation object for the planar model and set all the parameters
    pcl::SACSegmentation<pcl::PointXYZ> seg ;
    pcl::PointIndices::Ptr inliers (new pcl::PointIndices) ;
    pcl::ModelCoefficients::Ptr coefficients (new pcl::ModelCoefficients) ;
    pcl::PointCloud<pcl::PointXYZ>::Ptr cloud_plane (new pcl::PointCloud<pcl::PointXYZ> ()) ;
    pcl::PCDWriter writer ;
    /*an optimization is performed over the estimated coefficients to minimize or reduce
     * the mean-squared-error with respect the all of the 3D points.If you know you have outliers
     * an no noise,an optimization needn't set to save computational time .*/
    seg.setOptimizeCoefficients (true) ; //
    seg.setModelType (pcl::SACMODEL_PLANE) ;
    seg.setMethodType (pcl::SAC_RANSAC) ;
    seg.setMaxIterations (100) ;
    seg.setDistanceThreshold (0.02) ;

    int i=0, nr_points = (int) cloud_filtered->points.size ();
    //remove the planar 
    while (cloud_filtered->points.size () > 0.3 * nr_points){
        // Segment the largest planar component from the remaining cloud
        seg.setInputCloud (cloud_filtered);
        seg.segment (*inliers, *coefficients);
        if (inliers->indices.size () == 0){
            std::cout << "Could not estimate a planar model for the given dataset." << std::endl;
            break;
        }

        // Extract the planar inliers from the input cloud
        pcl::ExtractIndices<pcl::PointXYZ> extract;
        extract.setInputCloud (cloud_filtered);
        extract.setIndices (inliers);
        extract.setNegative (false);

        // Get the points associated with the planar surface
        extract.filter (*cloud_plane);
        std::cout << "PointCloud representing the planar component: " << cloud_plane->points.size () << " data points." << std::endl;

        // Remove the planar inliers, extract the rest
        extract.setNegative (true);
        extract.filter (*cloud_f);
        *cloud_filtered = *cloud_f;
    }

    // Creating the KdTree object for the search method of the extraction
    pcl::search::KdTree<pcl::PointXYZ>::Ptr tree (new pcl::search::KdTree<pcl::PointXYZ>);
    tree->setInputCloud (cloud_filtered);

    std::vector<pcl::PointIndices> cluster_indices;
    pcl::EuclideanClusterExtraction<pcl::PointXYZ> ec;
    ec.setClusterTolerance (0.02); // 2cm
    ec.setMinClusterSize (100);
    ec.setMaxClusterSize (25000);
    ec.setSearchMethod (tree);
    ec.setInputCloud (cloud_filtered);
    ec.extract (cluster_indices);

    int j = 0 ;
    for (std::vector<pcl::PointIndices>::const_iterator it = cluster_indices.begin (); it != cluster_indices.end (); ++it){
        pcl::PointCloud<pcl::PointXYZ>::Ptr cloud_cluster (new pcl::PointCloud<pcl::PointXYZ>);
        for (std::vector<int>::const_iterator pit = it->indices.begin (); pit != it->indices.end (); pit++)
            cloud_cluster->points.push_back (cloud_filtered->points[*pit]); 
        cloud_cluster->width = cloud_cluster->points.size ();
        cloud_cluster->height = 1;
        cloud_cluster->is_dense = true;

        std::cout << "PointCloud representing the Cluster: " << cloud_cluster->points.size () << " data points." << std::endl;
        std::stringstream ss;
        ss << "cloud_cluster_" << j << ".pcd";
        writer.write<pcl::PointXYZ> (ss.str (), *cloud_cluster, false); 
        j++;
    }

    return (0);
}
