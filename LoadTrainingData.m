fileNamePrefix="./DataSet/";
fileNames=["Normal.data"];
for idx=1:20
    if(idx<10)
        error_str=strcat('0',num2str(idx));
    else
        error_str=num2str(idx);
    end
    fileNames=[fileNames;strcat("Error",error_str,".data")];
end

FullDataSet=zeros(52,960*21);
fclose('all');
for idx=1:21
    fid=fopen(strcat(fileNamePrefix,fileNames(idx)),'rb');
    FullDataSet(:,((idx-1)*960+1):(idx*960))=fread(fid,[52,960],'double');
    fclose(fid);
end