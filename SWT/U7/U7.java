public class NotepadMinusMinus{
    private IStorage storage;
    
    public void save(){
        String content = DocBuffer.getContent();
        String name = DocManager.getCurrentFileName();
        storage.store(name, content);
    }
    
    public void setStorage(IStorage s){
        this.storage = s;
    }
}

public class DbxClient{
    public enum WriteMode{
        ADD, REPLACE
    }
    
    public boolean fileExists(String name){}
    
    public void uploadFile(String name, WriteMode mode, byte[] data){}
}

public class Dropbox implements IStorage{
    private DbxClient client = new DbxClient();
    
    public void store(String name, String content){
        DbxClient.WriteMode mode = WriteMode.ADD;
        if(client.fileExists(name)){
            mode = WriteMode.REPLACE;
        }
        client.uploadFile(name, mode, content.getBytes());
    }
}

public interface IStorage{
    public void store(String name, String content);
}

public class Storage implements IStorage{
    private Filesystem fs;
    
    public void store(String name, String content){
        fs.touch(name);
        fs.remove(name);
        fs.write(fs.position(name), content.getBytes());
    }
}
