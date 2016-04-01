%
% Plot a mesh specified in an SMF file
%
% -----------------------------------------------------
% Richard Zhang (c) 2016
%
function plot_smf(F, X)

    trimesh(F, X(:,1), X(:,2), X(:,3), 'EdgeColor', [0.3 0.3 0.3], ... 
                                       'FaceColor', [0.8 0.8 0.8], ...
                                       'FaceLighting', 'phong');
                                   
end