import numpy as np
import pandas as pd
# import laspy

# tools for visualization
from sklearn.manifold import TSNE
from sklearn.decomposition import PCA
from matplotlib import pyplot as plt
from mpl_toolkits.mplot3d import Axes3D

# tools for clustering
from sklearn.cluster import DBSCAN
from sklearn.cluster import AffinityPropagation
from sklearn.mixture import GaussianMixture
import hdbscan # https://github.com/scikit-learn-contrib/hdbscan

class Reader:
    def read_txt(self, file):
        self.current_file = pd.read_csv(file, sep=" ", header=None) # temp file
    def to_np(self):
        self.current_file = np.array(self.current_file)
    def normalization(self):
        self.to_np()
        temp = self.current_file[:,:-1].copy()
        self.current_file[:,:-1] = (temp - temp.mean(0))/temp.std(0)

class Visualizer:
    def pca(self, data, out_dim):
        self.data = PCA(n_components=out_dim).fit_transform(data)
    def tsne(self, data, out_dim, pca_dim=None, perplexity=15, verbose=0):
        if pca_dim is not None:
            self.pca(data, pca_dim)
            data = self.data.copy()
        self.data = TSNE(n_components=out_dim, perplexity=perplexity, verbose=verbose).fit_transform(data)
    def plot_data(self, conf=True, color=None):
        fig = plt.figure(figsize=(8,6))
        dims = self.data.shape[1]
        if conf:
            dims -= 1
            if color is None:
                color = self.data[:,-1]
        if dims==1:
            print("no implemented")
        elif dims==2:
            ax = fig.add_subplot()
            ax.set_aspect('equal')
            ax.scatter(self.data[:,0],self.data[:,1])
        elif dims==3:
            ax = Axes3D(fig)
            if conf: ax.scatter(self.data[:,0],self.data[:,1],self.data[:,2], c=color, s=(self.data[:,3]+1)**7, cmap="jet")
            else: ax.scatter(self.data[:,0],self.data[:,1],self.data[:,2], c=color, cmap="jet")
            ax.set_xlim3d(ax.get_xlim3d())
            ax.set_ylim3d(ax.get_ylim3d())
            ax.set_zlim3d(ax.get_zlim3d())
        else:
            print("bigger dims are not implemented")
        plt.show()

class Clustering:
    def __init__(self):
        print("implemented methods... \n DBSCAN \n AffinityPropagation \n HDBSCAN")
    def colorize_vector(self, labels):
        colors = [plt.cm.jet(val) for val in np.linspace(0, 1, len(np.unique(labels)))]
        self.colors = np.array(colors)[labels]        
    def clustering(self, data, method="DBSCAN"):
        print("clustering method used: ", method)
        method = method.lower()
        
        if method=='dbscan':
            algo = DBSCAN(eps=2, min_samples=20, leaf_size=30, algorithm="kd_tree").fit(data)
        elif method=="affinitypropagation":
            algo = AffinityPropagation(damping=0.5, max_iter=200, verbose=1).fit(data)
        elif method=='hdbscan':
            algo = hdbscan.HDBSCAN(cluster_selection_epsilon=0.2, min_cluster_size=20, min_samples=15, algorithm="best").fit(data)

        self.labels = algo.labels_
        self.n_clusters_ = len(set(self.labels)) - (1 if -1 in self.labels else 0)
        self.n_noise_ = list(self.labels).count(-1)
        print("number of clusters: ", self.n_clusters_)
        print("number of noise points: ", self.n_noise_)
        self.colorize_vector(self.labels)
    def __help__(self):
        print("implemented methods... \n DBSCAN \n AffinityPropagation \n HDBSCAN")



reader = Reader()
visualizer = Visualizer()
clusterer = Clustering()

reader.read_txt("../camera_lidar/point_cloud.txt")
# reader.normalization()
reader.to_np()

# print("reducing dimensions")
# visualizer.tsne(reader.current_file[:,:-1], 2, verbose=1)
# visualizer.plot_data(conf=False)

################ remove possible outliers
# visualizer.data = reader.current_file[reader.current_file[:,-1]>0.,:]

visualizer.data = reader.current_file
# visualizer.plot_data()

# clusterer.clustering(visualizer.data[:,:-1], method="DBSCAN")
clusterer.clustering(visualizer.data, method="DBSCAN")
visualizer.plot_data(color=clusterer.colors, conf=False)

import pandas as pd
labels = pd.read_csv("labels.txt")["labels"]
clusterer.colorize_vector(labels)
visualizer.plot_data(color=clusterer.colors, conf=False)