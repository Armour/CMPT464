function idx = findClosestCentroids(X, centroids)
% This function computes the centroid memberships for every example
% and returns the closest centroids in idx for a dataset X where each row
% is a single example.

% Initialize idx
idx = zeros(size(X, 1), 1);

% Go over every example, find its closest centroid, and store the index
% inside idx at the appropriate location
for i = 1 : length(idx)
    minDist = inf;
    for j = 1 : size(centroids, 1)
        jDist = (X(i, :) - centroids(j, :)) * (X(i, :) - centroids(j, :))';
        if jDist < minDist
            minDist = jDist;
            idx(i) = j;
        end
    end
end

end

