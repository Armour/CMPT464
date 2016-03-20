function centroids = updateCentroids(X, idx, K)
% This function find the new centroids by computing the means of the data
% points assigned to each centroid.

% Initialize centroids
centroids = zeros(K, size(X, 2));

% Go over every centroid and compute mean of all points that belong to it
for i = 1 : K
    chosen_idx = find(idx == i);
    centroids(i, :) = sum(X(chosen_idx , :)) / length(chosen_idx);
end

end

