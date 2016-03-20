function [centroids, idx] = kmeans(X, K, initg)
% This function runs the K-Means algorithm on data matrix x, where each
% row of x is a single example. It uses initial_centroids used as the
% initial centroids, max_iters specifies the total number of interactions
% of K-Means to execute.

% Check initg, if not set then random init centroids
if isempty(initg)
    initg = randomInitCentroids(X, K);
end

% Initialize values
idx = zeros(size(X, 1), 1);
centroids = initg;
previous_centroids = centroids / 2; % Make sure initial not the same
i = 0;

% Run K-Means
while previous_centroids ~= centroids
    % Output progress
    fprintf('K-Means iteration %d...\n', i);
    i = i + 1;

    % For each example in X, assign it to the closest centroid
    idx = findClosestCentroids(X, centroids);

    % Update previous_centroids
    previous_centroids = centroids;

    % Given the memberships, update new centroids
    centroids = updateCentroids(X, idx, size(initg, 1));
end

% Plot result
res = gscatter(X(:, 1), X(:, 2), idx);
set(res(:), 'MarkerSize', 25);
title('K-Means Clustring Result');
xlabel('x-axis');
ylabel('y-axis');

end

