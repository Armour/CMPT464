function centroids = randomInitCentroids(X, K)
% This function initializes K centroids that are to be used in K-Means on 
% the dataset X and returns K initial centroids to be used with the K-Means
% on the dataset X

% Randomly reorder the indices of examples
randidx = randperm(size(X, 1));

% Take the first K examples as centroids
centroids = X(randidx(1: K), :);

end

