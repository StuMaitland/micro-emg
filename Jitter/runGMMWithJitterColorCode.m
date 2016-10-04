%close all
options = statset('MaxIter',100); % Increase number of EM iterations
outputImage = zeros(1400,400);
outputImage (floor(ex*100),floor((ey+2)*100))=1;

if ~exist('clusterFigure')
    clusterFigure=figure;
    set(clusterFigure,'Color',[1 1 1])
    set(clusterFigure,'Units','normal')
    set(clusterFigure,'OuterPosition',[0 0 1 1]);
    set(clusterFigure, 'menubar', 'none');
    scatter(ex,ey,'s','fill');
    hold on
    axis off
    axis([0 14 -3 3])
    if recordMovie==1
        writerObj = VideoWriter('MUs3.avi');
        writerObj.FrameRate = 3;
        open(writerObj);
    end

else
    figure(clusterFigure);
end

for pi=1:length(unique(ptsC))-1
    col = assignedColor(pi,:);
    X= locations;
    X = X(ptsC==pi,:);
    if length(X)<=2
        continue;
    end
    d = 1000;
    x1 = linspace(min(X(:,1)) - 2,max(X(:,1)) + 2,d);
    x2 = linspace(min(X(:,2)) - 2,max(X(:,2)) + 2,d);
    [x1grid,x2grid] = meshgrid(x1,x2);
    X0 = [x1grid(:) x2grid(:)];
    threshold = sqrt(chi2inv(0.99,2));

    k = 1;%size(centers,2);
    S.mu = centers(:,pi)';
    S.Sigma = zeros(2,2,k);
    S.ComponentProportion = zeros(1,k);
    for j=1:k
        a = locations(ptsC==j,:);
        S.Sigma(:,:,j)=cov(a);
        S.ComponentProportion(j) = size(a,1)/size(X,1);
    end
    Sigma = {'full'};
    nSigma = numel(Sigma);
    SharedCovariance = {false};
    nSC = numel(SharedCovariance);

    for i = 1:nSigma;
        for j = 1:nSC;
            gmfit = fitgmdist(X,k,'CovarianceType',Sigma{i},...
                'SharedCovariance',SharedCovariance{j},'Options',options);
            clusterX = cluster(gmfit,X);
            mahalDist = mahal(gmfit,X0);
            if fixedColor
                if recordMovie==0
                    for sp=1:size(X,1)
                        h1 = gscatter(X(sp,1),X(sp,2),clusterX(sp),col,'.',5,'off');
                        pause(paintDelay);
                    end
                else
                    for xi=1:size(X,1)
                        h1=gscatter(X(xi,1),X(xi,2),clusterX(xi,:),col,'.',5,'off');
                         F = getframe(clusterFigure);
                         writeVideo(writerObj,F);
                    end
                end
            else
                h1 = gscatter(X(:,1),X(:,2),clusterX,[rand rand rand],'.',5,'off');
            end
            hold on;
                for m = 1:k;
                    idx = mahalDist(:,m)<=threshold+6;
                    Color = h1(m).Color*0.75 + -0.5*(h1(m).Color - 1);
                    h2 = plot(X0(idx,1),X0(idx,2),'.','Color',Color,'MarkerSize',1);
                    uistack(h2,'bottom');
                    F = getframe(clusterFigure);
                    if recordMovie==1
                        writeVideo(writerObj,F);
                    end
                end
            X0Temp = X0(idx,:);
            mahalDistTemp = mahalDist(idx);
            mxDist = max(mahalDistTemp);
            for t=1:length(mahalDistTemp)
                outputImage(max(1,floor(X0Temp(t,1)*100)),max(1,floor((X0Temp(t,2)+2)*100)))=mxDist-mahalDistTemp(t);
            end
            
            plot(S.mu(:,1),S.mu(:,2),'kx','LineWidth',2,'MarkerSize',2)
            if recordMovie==1
                F = getframe(clusterFigure);
                writeVideo(writerObj,F);
            end
            %axis([0 14 -2.2 2.2])
        end
    end
end
if plotMU==1 && length(unique(ptsC))>1
    circle(muscle.MUSC(mu,1),muscle.MUSC(mu,2),muscle.MUSR(mu),col);
end

if recordMovie==1
    F = getframe(clusterFigure);
    writeVideo(writerObj,F);
end

if mu==10 && recordMovie==1
    close(writerObj);
end
% figure;
% colormap(jet)
% imagesc(outputImage');
% hold on
% scatter(floor(ex*100),floor((ey+2)*100),'sw','fill');