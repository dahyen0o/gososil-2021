// 채팅 키워드 검색창 & 검색 버튼에 대한 컴포넌트 작성
import React, { useState } from "react";
import styled from "styled-components";
import { TextField } from "@material-ui/core";

function ChatSearch({onSearchKeySubmit}){
    const [searchState, setSearchState] = useState("");

    const onMsgChange = (e) => {
        setSearchState(e.target.value)
        console.log(e.target.value)
    }

    const onMsgSubmit = (e) => {
        e.preventDefault()
        onSearchKeySubmit(searchState)
        setTimeout(1000)
        setSearchState("")

    }

  return (
    <Form onSubmit={onMsgSubmit}>
      <MessageContents>
        <TextField
          name="keyword"
          onChange={(e) => onMsgChange(e)}
          value={searchState}
          id="outlined-multiline-static"
          variant="outlined"
          label="keyword"
        />
      </MessageContents>
      <Button>Search</Button>
    </Form>
  );
}

const Form = styled.form`
  display: grid;
  grid-template-columns: 1fr 1fr;
`;


const MessageContents = styled.div`
  margin: 30px;
`;

const Button = styled.button`
  margin-left: 50px;
  margin-top: 30px;
  width: 80px;
  height: 50px;
  padding: 10px;
  background-color: white;
  border-color: #c8a951;
  border-radius: 5px;
`;

export default ChatSearch;