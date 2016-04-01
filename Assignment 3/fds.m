function fds(smf_file, level)
% This function runs the spectral mesh compression using Fourier
% descriptors, smf_file is the input mesh file name, level is a array of
% integer indicate the number of used eigenvectors.

% Read and plot original mesh
[F, X] = read_smf(smf_file);
figure;
plot_smf(F, X);
title('Original mesh');
xlabel('x-axis');
ylabel('y-axis');
zlabel('z-axis');

% Calculate K matrix
K = zeros(size(X, 1), size(X, 1));
for i = 1 : size(F, 1)
    v = F(i, :);
    for j = 1 : 3
        K(v(j), v(j)) = K(v(j), v(j)) + 1;
        K(v(j), v(mod(j, 3) + 1)) = -1;
        K(v(mod(j, 3) + 1), v(j)) = -1;
    end
end

% Get eigenvectors in order
[eig_vectors, eig_value] = eig(K);
eig_vectors = sortrows([diag(eig_value)'; eig_vectors]', 1)';
eig_vectors = eig_vectors(2:end, :);

% Fourier transform & Plot result
for i = 1 : length(level)
    dim = level(i);
    if dim > size(eig_vectors, 1)
        msg = ['Number (', num2str(dim), ') exceeds eigenvector matrix dimensions!'];
        disp(msg);
        dim = size(eig_vectors, 1);
    end
    Q = eig_vectors(:, 1:dim);
    figure;
    plot_smf(F, Q * Q' * X);
    title(['Compressed mesh with first ', num2str(dim), ' eigenvectors.']);
    xlabel('x-axis');
    ylabel('y-axis');
    zlabel('z-axis');
end
    
end