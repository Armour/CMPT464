% ======================= K-Means Clustering ==========================

% Load an example dataset
X = load('kmeans_data/gauss_data_1.txt');

% Settings for running K-Means
K = 6;

% Run K-Means algorithm without inital centroids
[centroids, idx] = kmeans(X, K, []);
