% ======================= K-Means Clustering ==========================

% Load an example dataset
X = load('kmeans_data/gauss_data_2.txt');

% Settings for running K-Means
K = 5;

% Run K-Means algorithm without inital centroids
[centroids, idx] = kmeans(X, K, []);